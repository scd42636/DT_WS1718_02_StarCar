// -------------------------------------------------------------------------------------------------
// <copyright file="Timer.c" company="OTH Regensburg">
//     Copyright (c) OTH Regensburg, Algorithmen f�r Sensornetze. All rights reserved.
// </copyright>
// <autor>Dominik Scharnagl (MatNr. 3054541)</autor>
// -------------------------------------------------------------------------------------------------

#include "Timer.h"
#include "Accelerator.h"
#include "Radio.h"
#include "MotionControl.h"


// ---------- Private fields ----------

Action(PeriodicTick);


// ---------- Private methods (private predefines) ----------

void Timer0_Init();


// ---------- Public methods (public defines) ----------

// Wait for some microseconds
// ticks: (1 tick = 1/32768 sec)
// Original: Timer_0_A4_Delay(...)
void Timer_Delay(u16 ticks)
{
    u16 value = 0;

    // Exit immediately if Timer0 not running - otherwise we'll get stuck here
    if ((TA0CTL & (BIT4 | BIT5)) == 0)
        return;

    // Disable timer interrupt
    TA0CCTL4 &= ~CCIE;

    // Clear delay_over flag
    System.States.DelayOver = false;

    // Add delay to current timer value
    // To make sure this value is correctly read
    while (value != TA0R)
        value = TA0R;

    value += ticks;

    // Update CCR
    TA0CCR4 = value;

    // Reset IRQ flag
    TA0CCTL4 &= ~CCIFG;

    // Enable timer interrupt
    TA0CCTL4 |= CCIE;

    // Wait for timer IRQ
    while (1)
    {
        // Delay in LPM
        // Watch_GoIntoLowPowerMode()
        // Go to LPM3
        _BIS_SR(LPM3_bits + GIE); // will also set GIE again
        __no_operation();

#ifdef USE_WATCHDOG
        // Service watchdog
        WDTCTL = WDTPW + WDTIS__512K + WDTSSEL__ACLK + WDTCNTCL;
#endif

        // Check stop condition
        // disable interrupt to prevent flag's change caused by interrupt methods
        __disable_interrupt();
        if (System.States.DelayOver)
            break;
    }
    __enable_interrupt();
}

// Original: Timer0_Start
void Timer_Start()
{
    // Start Timer0 in continuous mode
    TA0CTL |= MC_2;
}

// Original: Timer0_Stop
void Timer_Stop()
{
    // Stop Timer0
    TA0CTL &= ~MC_2;

    // Set Timer0 count register to 0x0000
    TA0R = 0;
}

// Original: Timer0_Init
void Timer_Setup()
{
    PeriodicTick = nullptr;

    // Set Timer0 to a period of 1 or 2 sec. IRQ TACCR0 is asserted when timer overflows.
    {
        // Configure Timer0 for use by the clock and delay functions
        // Set interrupt frequency to 1Hz
        TA0CCR0 = 32768 - 1;

        // Enable timer interrupt
        TA0CCTL0 |= CCIE;

        // Clear and start timer now
        // Continuous mode: Count to 0xFFFF and restart from 0 again - 1sec timing will be generated by
        // ISR
        TA0CTL |= TASSEL0 + MC1 + TACLR;
    }
}

// Trigger IRQ every "ticks" microseconds
// ticks: (1 tick = 1/32768 sec)
// Original: Timer0_A3_Start(...)
void Timer_StartPeriodic(u16 ticks, ActionFunction callback)
{
    PeriodicTick = callback;
    u16 value = 0;

    // Store timer ticks in global variable
    SystemTimer.Ticks = ticks;

    // Delay based on current counter value
    // To make sure this value is correctly read
    while (value != TA0R)
        value = TA0R;
    value += ticks;

    // Update CCR
    TA0CCR3 = value;

    // Reset IRQ flag
    TA0CCTL3 &= ~CCIFG;

    // Enable timer interrupt
    TA0CCTL3 |= CCIE;
}

// Original: Timer0_A3_Stop(...)
void Timer_StopPeriodic()
{
    // Clear timer interrupt
    TA0CCTL3 &= ~CCIE;
}

// ---------- Private methods (private defines) ----------

// Interrupt service routine
//          Timer0_A0       1/1sec clock tick                       (serviced by function TIMER0_A0_ISR)
//          Timer0_A1                                               (serviced by function TIMER0_A1_5_ISR)
//          Timer0_A2       1/100 sec Stopwatch                     (serviced by function TIMER0_A1_5_ISR)
//          Timer0_A3       Configurable periodic IRQ               (serviced by function TIMER0_A1_5_ISR)
//          Timer0_A4       One-time delay                          (serviced by function TIMER0_A1_5_ISR)
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
    // Disable IE
    TA0CCTL0 &= ~CCIE;
    // Reset IRQ flag
    TA0CCTL0 &= ~CCIFG;
    // Add 1 sec to TACCR0 register (IRQ will be asserted at 0x7FFF and 0xFFFF = 1 sec intervals)
    TA0CCR0 += 32768;
    // Enable IE
    TA0CCTL0 |= CCIE;

    // While SimpliciTI stack operates searches, freeze system state
    if (Radio_IsOpen()) // is_rf()
    {
        // Motion Control automatic timeout
        if (MotionControl.Timeout == 0)
        {
            MotionControl.Command = MotionControlCommand_Stop;
        }
        else
        {
            MotionControl.Timeout--;
        }

        // Exit from LPM3 on RETI
        _BIC_SR_IRQ(LPM3_bits);
        return;
    }

    // -------------------------------------------------------------------
    // Service active modules that require 1/s processing

    // Count down timeout
    if (Accelerator_IsActive())
    {
        // Countdown acceleration measurement timeout
        SystemAccelerator.Timeout--;

        // Stop measurement when timeout has elapsed
        if (SystemAccelerator.Timeout == 0)
        {
            Accelerator_Stop();
        }

        // If DRDY is (still) high, request data again
        if ((AS_INT_IN & AS_INT_PIN) == AS_INT_PIN)
            SystemRequests.States.MeasureAcceleration = true;
    }

    // Exit from LPM3 on RETI
    _BIC_SR_IRQ(LPM3_bits);
}

// Interrupt service routine
//      Timer0_A0       1/1sec clock tick (serviced by function TIMER0_A0_ISR)
//      Timer0_A1       BlueRobin timer
//      Timer0_A2       1/100 sec Stopwatch
//      Timer0_A3       Configurable periodic IRQ (used by button_repeat and buzzer)
//      Timer0_A4       One-time delay
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_5_ISR(void)
{
    u16 value = 0;

    switch (TA0IV)
    {
        // Timer0_A1    BlueRobin timer
        case 0x02:             // Timer0_A1 handler
            ////BRRX_TimerTask_v();
            break;

        // Timer0_A2    1/1 or 1/100 sec Stopwatch
        case 0x04:             // Timer0_A2 handler
            // Disable IE
            TA0CCTL2 &= ~CCIE;
            // Reset IRQ flag
            TA0CCTL2 &= ~CCIFG;
            // Enable timer interrupt
            TA0CCTL2 |= CCIE;
            break;

        // Timer0_A3    Configurable periodic IRQ (used by button_repeat and buzzer)
        case 0x06:             // Disable IE
            TA0CCTL3 &= ~CCIE;
            // Reset IRQ flag
            TA0CCTL3 &= ~CCIFG;

            // Store new value in CCR
            // To make sure this value is correctly read
            while (value != TA0R)
                value = TA0R;

            value += SystemTimer.Ticks;

            // Load CCR register with next capture point
            TA0CCR3 = value;
            // Enable timer interrupt
            TA0CCTL3 |= CCIE;
            // Call function handler
            PeriodicTick();
            break;

        // Timer0_A4    One-time delay
        case 0x08:             // Disable IE
            TA0CCTL4 &= ~CCIE;
            // Reset IRQ flag
            TA0CCTL4 &= ~CCIFG;
            // Set delay over flag
            System.States.DelayOver = true;
            break;
    }

    // Exit from LPM3 on RETI
    _BIC_SR_IRQ(LPM3_bits);
}