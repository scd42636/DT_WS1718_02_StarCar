// -------------------------------------------------------------------------------------------------
// <copyright file="Radio.c" company="OTH Regensburg">
//     Copyright (c) OTH Regensburg, Algorithmen für Sensornetze. All rights reserved.
// </copyright>
// <autor>Dominik Scharnagl (MatNr. 3054541)</autor>
// -------------------------------------------------------------------------------------------------

#include "Radio.h"


// ---------- Public methods (public defines) ----------

bool Radio_IsOpen()
{
    return HasFlag(RF1AIE, BIT4);
}

void Radio_Close()
{
    // Disable radio IRQ
    RF1AIFG = 0;
    RF1AIE = 0;

    // Reset radio core
    Radio_Reset();

    // Put radio to sleep
    Radio_Powerdown();
}

void Radio_Open()
{
    // Reset radio core
    Radio_Reset();

    // Enable radio IRQ
    RF1AIFG &= ~BIT4;           // Clear a pending interrupt
    RF1AIE |= BIT4;             // Enable the interrupt
}

void Radio_Powerdown()
{
    // Chip bug: Radio does not come out of this SLEEP when put to sleep
    // using the SPWD cmd. However, it does wakes up if SXOFF was used to
    // put it to sleep.

    // Powerdown radio
    Strobe(RF_SIDLE);
    Strobe(RF_SPWD);
}

void Radio_Reset()
{
    volatile u16 i;
    u8 x;

    // Reset radio core
    Strobe(RF_SRES);

    // Wait before checking IDLE
    for (i = 0; i < 100; i++);

    do
    {
        x = Strobe(RF_SIDLE);
    }
    while ((x & 0x70) != 0x00);

    // Clear radio error register
    RF1AIFERR = 0;
}

void Radio_Setup()
{
    Radio_Reset();
    Radio_Powerdown();
}

// ---------- Private methods (private defines) ----------

// Interrupt service routine
// - In SimpliciTI mode: go to SimpliciTI handler
#pragma vector=CC1101_VECTOR
__interrupt void radio_ISR(void)
{
    u8 rf1aivec = RF1AIV;

    if (Radio_IsOpen()) // is_rf()
    {
        MRFI_RadioIsr();
    }
    else
    {
        if (rf1aivec == RF1AIV_NONE) // RF1A interface interrupt (error etc.)
        {
            asm ("	nop"); // break here
        }
    }
}
