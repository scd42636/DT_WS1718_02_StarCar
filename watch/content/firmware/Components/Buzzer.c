// -------------------------------------------------------------------------------------------------
// <copyright file="Buzzer.c" company="OTH Regensburg">
//     Copyright (c) OTH Regensburg, Algorithmen für Sensornetze. All rights reserved.
// </copyright>
// <autor>Dominik Scharnagl (MatNr. 3054541)</autor>
// -------------------------------------------------------------------------------------------------

#include "Buzzer.h"
#include "Timer.h"


// ---------- Public methods (public defines) ----------

void Buzzer_Countdown()
{
    // Stop buzzer when reaching 0 cycles
    if (--SystemBuzzer.time == 0)
    {
        Buzzer_Stop();
    }
}

bool Buzzer_IsActive()
{
    return (SystemBuzzer.state != BUZZER_OFF);
}

void Buzzer_Reset()
{
    SystemBuzzer.time = 0;
    SystemBuzzer.state = BUZZER_OFF;
}

void Buzzer_Setup()
{
}

// u8 cycles       Keep buzzer output for number of cycles
// u16 on_time     Output buzzer for "on_time" ACLK ticks
// u16 off_time    Do not output buzzer for "off_time" ACLK ticks
void Buzzer_Start(u8 cycles, u16 on_time, u16 off_time)
{
    // Store new buzzer duration while buzzer is off
    if (SystemBuzzer.time == 0)
    {
        SystemBuzzer.time = cycles;
        SystemBuzzer.on_time = on_time;
        SystemBuzzer.off_time = off_time;

        // Need to init every time, because SimpliciTI claims same timer
        // Reset TA1R, set up mode, TA1 runs from 32768Hz ACLK
        TA1CTL = TACLR | MC_1 | TASSEL__ACLK;

        // Set PWM frequency
        TA1CCR0 = BUZZER_TIMER_STEPS;

        // Enable IRQ, set output mode "toggle"
        TA1CCTL0 = OUTMOD_4;

        // Allow buzzer PWM output on P2.7
        P2SEL |= BIT7;

        // Activate Timer0_A3 periodic interrupts
        Timer_StartPeriodic(SystemBuzzer.on_time, Buzzer_Toggle);

        // Preload timer advance variable
        SystemTimer.Ticks = SystemBuzzer.off_time;

        // Start with buzzer output on
        SystemBuzzer.state = BUZZER_ON_OUTPUT_ENABLED;
    }
}

void Buzzer_Stop()
{
    // Stop PWM timer
    TA1CTL &= ~(BIT4 | BIT5);

    // Disable buzzer PWM output
    P2OUT &= ~BIT7;
    P2SEL &= ~BIT7;

    // Clear PWM timer interrupt
    TA1CCTL0 &= ~CCIE;

    Timer_StopPeriodic();
    Buzzer_Reset();
}

void Buzzer_Toggle()
{
    // Turn off buzzer
    if (SystemBuzzer.state == BUZZER_ON_OUTPUT_ENABLED)
    {
        // Stop PWM timer
        TA1CTL &= ~(BIT4 | BIT5);

        // Reset and disable buzzer PWM output
        P2OUT &= ~BIT7;
        P2SEL &= ~BIT7;

        // Update buzzer state
        SystemBuzzer.state = BUZZER_ON_OUTPUT_DISABLED;

        // Reload Timer0_A4 IRQ to restart output
        SystemTimer.Ticks = SystemBuzzer.on_time;
    }
    else                        // Turn on buzzer
    {
        // Decrement buzzer total cycles
        Buzzer_Countdown();

        // Reload Timer0_A4 to stop output if sBuzzer.time > 0
        if (SystemBuzzer.state != BUZZER_OFF)
        {
            // Reset timer TA1
            TA1R = 0;
            TA1CTL |= MC_1;

            // Enable buzzer PWM output
            P2SEL |= BIT7;

            // Update buzzer state
            SystemBuzzer.state = BUZZER_ON_OUTPUT_ENABLED;

            // Reload Timer0_A4 IRQ to turn off output
            SystemTimer.Ticks = SystemBuzzer.off_time;
        }
    }
}
