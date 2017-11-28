// -------------------------------------------------------------------------------------------------
// <copyright file="firmware.c" company="Sprocket Enterprises">
//     Copyright (c) OTH Regensburg, Team 02 StarCar. All rights reserved.
// </copyright>
// <autor>Dominik Scharnagl</autor>
// -------------------------------------------------------------------------------------------------

#include "firmware.h"

#include "io430.h"
#include "simpliciti.h"

void SetupFirmware()
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
}
