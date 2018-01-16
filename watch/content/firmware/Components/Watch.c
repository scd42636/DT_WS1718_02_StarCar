// -------------------------------------------------------------------------------------------------
// <copyright file="Watch.c" company="OTH Regensburg">
//     Copyright (c) OTH Regensburg, Algorithmen für Sensornetze. All rights reserved.
// </copyright>
// <autor>Dominik Scharnagl (MatNr. 3054541)</autor>
// -------------------------------------------------------------------------------------------------

#include "Watch.h"


// ---------- Public fields ----------

volatile SystemInfo System;
volatile ButtonInfo SystemButtons;
volatile RequestInfo SystemRequests;

AcceleratorInfo SystemAccelerator;
BatteryInfo SystemBattery;
BuzzerInfo SystemBuzzer;
SystemDataInfo SystemData;
TimerInfo SystemTimer;

void (*ButtonInterruptReceived)();


// ---------- Private methods (private predefines) ----------

void SetVCore(byte level);
void SetVCoreDown(byte level);
void SetVCoreUp(byte level);


// ---------- Public methods (public defines) ----------

void Watch_GoIntoIdleMode()
{
    // To low power mode
    Watch_GoIntoLowPowerMode();

#ifdef USE_WATCHDOG
    // Service watchdog
    WDTCTL = WDTPW + WDTIS__512K + WDTSSEL__ACLK + WDTCNTCL;
#endif
}

void Watch_GoIntoLowPowerMode()
{
    // Go to LPM3
    _BIS_SR(LPM3_bits + GIE);
    __no_operation();
}

void Watch_Setup()
{
    ButtonInterruptReceived = nullptr;

    System.Flags = 0;
    SystemButtons.Flags = 0;
    SystemRequests.Flags = 0;

    // Start with battery voltage of 3.00V
    SystemBattery.Voltage = 300;

#ifndef ISM_US
    // Use metric units when displaying values
    System.States.UseMetricUnits = true;
#endif

    volatile unsigned char *ptr;

    // ---------------------------------------------------------------------
    // Enable watchdog

    // Watchdog triggers after 16 seconds when not cleared
#ifdef USE_WATCHDOG
    WDTCTL = WDTPW + WDTIS__512K + WDTSSEL__ACLK;
#else
    WDTCTL = WDTPW + WDTHOLD;
#endif

    // ---------------------------------------------------------------------
    // Configure PMM
    SetVCore(3);

    // Set global high power request enable
    PMMCTL0_H = 0xA5;
    PMMCTL0_L |= PMMHPMRE;
    PMMCTL0_H = 0x00;

    // ---------------------------------------------------------------------
    // Enable 32kHz ACLK
    P5SEL |= 0x03;              // Select XIN, XOUT on P5.0 and P5.1
    UCSCTL6 &= ~XT1OFF;         // XT1 On, Highest drive strength
    UCSCTL6 |= XCAP_3;          // Internal load cap

    UCSCTL3 = SELA__XT1CLK;     // Select XT1 as FLL reference
    UCSCTL4 = SELA__XT1CLK | SELS__DCOCLKDIV | SELM__DCOCLKDIV;

    // ---------------------------------------------------------------------
    // Configure CPU clock for 12MHz
    _BIS_SR(SCG0);              // Disable the FLL control loop
    UCSCTL0 = 0x0000;           // Set lowest possible DCOx, MODx
    UCSCTL1 = DCORSEL_5;        // Select suitable range
    UCSCTL2 = FLLD_1 + 0x16E;   // Set DCO Multiplier
    _BIC_SR(SCG0);              // Enable the FLL control loop

    // Worst-case settling time for the DCO when the DCO range bits have been
    // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
    // UG for optimization.
    // 32 x 32 x 12 MHz / 32,768 Hz = 375000 = MCLK cycles for DCO to settle
    __delay_cycles(375000);

    // Loop until XT1 & DCO stabilizes, use do-while to insure that
    // body is executed at least once
    do
    {
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
        SFRIFG1 &= ~OFIFG;      // Clear fault flags
    }
    while ((SFRIFG1 & OFIFG));

    // ---------------------------------------------------------------------
    // Configure port mapping

    // Disable all interrupts
    __disable_interrupt();
    // Get write-access to port mapping registers:
    PMAPPWD = 0x02D52;
    // Allow reconfiguration during runtime:
    PMAPCTL = PMAPRECFG;

    // P2.7 = TA0CCR1A or TA1CCR0A output (buzzer output)
    ptr = &P2MAP0;
    *(ptr + 7) = PM_TA1CCR0A;
    P2OUT &= ~BIT7;
    P2DIR |= BIT7;

    // P1.5 = SPI MISO input
    ptr = &P1MAP0;
    *(ptr + 5) = PM_UCA0SOMI;
    // P1.6 = SPI MOSI output
    *(ptr + 6) = PM_UCA0SIMO;
    // P1.7 = SPI CLK output
    *(ptr + 7) = PM_UCA0CLK;

    // Disable write-access to port mapping registers:
    PMAPPWD = 0;
    // Re-enable all interrupts
    __enable_interrupt();

    // ---------------------------------------------------------------------
    // Init buttons
    // Set button ports to input
    BUTTONS_DIR &= ~ALL_BUTTONS;

    // Enable internal pull-downs
    BUTTONS_OUT &= ~ALL_BUTTONS;
    BUTTONS_REN |= ALL_BUTTONS;

    // IRQ triggers on rising edge
    BUTTONS_IES &= ~ALL_BUTTONS;

    // Reset IRQ flags
    BUTTONS_IFG &= ~ALL_BUTTONS;

    // Enable button interrupts
    BUTTONS_IE |= ALL_BUTTONS;

    // Init LCD
    lcd_init();
}

void Watch_ShowWelcome()
{
    // Disable timer - no need for a clock tick
    Timer_Stop();

    // Disable LCD charge pump while in standby mode
    // This reduces current consumption by ca. 5µA to ca. 10µA
    LCDBVCTL = 0;

    // Show welcome screen
    display_chars(LCD_SEG_L1_2_0, (u8 *) "Car", SEG_ON);
    display_chars(LCD_SEG_L2_4_0, (u8 *) "HELLO", SEG_ON);
    display_symbol(LCD_ICON_STOPWATCH, SEG_ON);
    display_symbol(LCD_ICON_RECORD, SEG_ON);
    display_symbol(LCD_ICON_BEEPER1, SEG_ON);
    display_symbol(LCD_ICON_BEEPER2, SEG_ON);
    display_symbol(LCD_ICON_BEEPER3, SEG_ON);

    // Hold watchdog
    WDTCTL = WDTPW + WDTHOLD;

    // Wait for button press
    _BIS_SR(LPM3_bits + GIE);
    __no_operation();

    // Clear display
    display_all_off();

#ifdef USE_LCD_CHARGE_PUMP
    // Charge pump voltage generated internally, internal bias (V2-V4) generation
    // This ensures that the contrast and LCD control is constant for the whole battery lifetime
    LCDBVCTL = LCDCPEN | VLCD_2_72;
#endif

    // Renenable timer
    Timer_Start();

    // Debounce button press
    Timer_Delay(ConvertMillisecondsToTicks(100));
}

// ---------- Private methods (private defines) ----------

// Note: change level by one step only.
void SetVCore(byte level)
{
    byte actLevel = 0;

    do
    {
        actLevel = PMMCTL0_L & PMMCOREV_3;

        if (actLevel < level)
            SetVCoreUp(++actLevel);                                  // Set VCore (step by step)

        if (actLevel > level)
            SetVCoreDown(--actLevel);                                // Set VCore (step by step)
    }
    while (actLevel != level);
}

void SetVCoreDown(byte level)
{
    PMMCTL0_H = 0xA5;                                                // Open PMM module registers
                                                                     // for write access
    SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level; // Set SVS/M Low side to new
                                                                     // level
    while ((PMMIFG & SVSMLDLYIFG) == 0) ;                            // Wait till SVM is settled
                                                                     // (Delay)
    PMMCTL0_L = (level * PMMCOREV0);                                 // Set VCore to 1.85 V for Max
                                                                     // Speed.
    PMMCTL0_H = 0x00;                                                // Lock PMM module registers
                                                                     // for write access
}

// Note: change level by one step only.
void SetVCoreUp(byte level)
{
    // Open PMM module registers for write access
    PMMCTL0_H = 0xA5;

    // Set SVS/M high side to new level
    SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;

    SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;                    // Set SVM new Level
    while ((PMMIFG & SVSMLDLYIFG) == 0) ;                            // Wait till SVM is settled
                                                                     // (Delay)
    PMMCTL0_L = PMMCOREV0 * level;                                   // Set VCore to x
    PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);                               // Clear already set flags
    if ((PMMIFG & SVMLIFG))
        while ((PMMIFG & SVMLVLRIFG) == 0) ;                         // Wait till level is reached

    SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level; // Set SVS/M Low side to new
                                                                     // level
    PMMCTL0_H = 0x00;                                                // Lock PMM module registers
                                                                     // for write access
}

// Interrupt service routine for
// - buttons
// - acceleration sensor INT1
#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
    // Clear flags
    u8 int_flag, int_enable;

    // Remember interrupt enable bits
    int_enable = BUTTONS_IE;

    // Clear button flags
    SystemButtons.Flags = 0;

    // Store valid button interrupt flag
    int_flag = BUTTONS_IFG & int_enable;

    if (ButtonInterruptReceived != nullptr)
        ButtonInterruptReceived();

    // Debounce buttons
    if ((int_flag & ALL_BUTTONS) != 0)
    {
        // Disable PORT2 IRQ
        __disable_interrupt();
        BUTTONS_IE = 0x00;
        __enable_interrupt();

        // Debounce delay 1
        Timer_Delay(ConvertMillisecondsToTicks(BUTTONS_DEBOUNCE_TIME_IN));
    }

    // ---------------------------------------------------
    // STAR button IRQ
    if (HasFlag(int_flag, BUTTON_STAR_PIN))
    {
        // Filter bouncing noise
        if (ButtonIsStarPressed)
        {
            SystemButtons.States.HasStarPressed = true;
        }
        else if ((BUTTONS_IES & BUTTON_STAR_PIN) == BUTTON_STAR_PIN)
        {
            SystemButtons.States.HasStarPressed = true;
            BUTTONS_IES &= ~BUTTON_STAR_PIN;
        }
    }
    // ---------------------------------------------------
    // NUM button IRQ
    else if (HasFlag(int_flag, BUTTON_NUM_PIN))
    {
        // Filter bouncing noise
        if (ButtonIsNumPressed)
        {
            SystemButtons.States.HasNumPressed = true;
        }
        else if ((BUTTONS_IES & BUTTON_NUM_PIN) == BUTTON_NUM_PIN)
        {
            SystemButtons.States.HasNumPressed = true;
            BUTTONS_IES &= ~BUTTON_NUM_PIN;
        }
    }
    // ---------------------------------------------------
    // UP button IRQ
    else if (HasFlag(int_flag, BUTTON_UP_PIN))
    {
        // Filter bouncing noise
        if (ButtonIsUpPressed)
        {
            SystemButtons.States.HasUpPressed = true;
        }
    }
    // ---------------------------------------------------
    // DOWN button IRQ
    else if (HasFlag(int_flag, BUTTON_DOWN_PIN))
    {
        // Filter bouncing noise
        if (ButtonIsDownPressed)
        {
            SystemButtons.States.HasDownPressed = true;
        }
    }

    // Trying to lock/unlock buttons?
    if (SystemButtons.States.HasNumPressed && SystemButtons.States.HasDownPressed)
    {
        SystemButtons.Flags = 0;
    }

    // ---------------------------------------------------
    // Acceleration sensor IRQ
    if (HasFlag(int_flag, AS_INT_PIN))
    {
        // Get data from sensor
        SystemRequests.States.MeasureAcceleration = true;
    }

    // ---------------------------------------------------
    // Safe long button event detection
    if (SystemButtons.States.HasStarPressed || SystemButtons.States.HasNumPressed)
    {
        // Additional debounce delay to enable safe high detection - 50ms
        Timer_Delay(ConvertMillisecondsToTicks(BUTTONS_DEBOUNCE_TIME_LEFT));

        // Check if this button event is short enough
        if (ButtonIsStarPressed)
        {
            // Change interrupt edge to detect button release
            BUTTONS_IES |= BUTTON_STAR_PIN;
            SystemButtons.States.HasStarPressed = false;
        }
        if (ButtonIsNumPressed)
        {
            // Change interrupt edge to detect button release
            BUTTONS_IES |= BUTTON_NUM_PIN;
            SystemButtons.States.HasNumPressed = false;
        }
    }

    // Reenable PORT2 IRQ
    __disable_interrupt();
    BUTTONS_IFG = 0x00;
    BUTTONS_IE = int_enable;
    __enable_interrupt();

    // Exit from LPM3/LPM4 on RETI
    __bic_SR_register_on_exit(LPM4_bits);
}
