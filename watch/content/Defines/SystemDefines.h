// -------------------------------------------------------------------------------------------------
// <copyright file="SystemDefines.h" company="OTH Regensburg">
//     Copyright (c) OTH Regensburg, Algorithmen für Sensornetze. All rights reserved.
// </copyright>
// <autor>Dominik Scharnagl (MatNr. 3054541)</autor>
// -------------------------------------------------------------------------------------------------

#ifndef SYSTEMDEFINES_H
#define SYSTEMDEFINES_H

#include "Defines.h"


typedef union
{
   struct
   {
      u16 IdleTimeout:1;           // Timeout after inactivity
      u16 IdleTimeoutEnabled:1;   // When in set mode, timeout after a given period
      u16 MaskBuzzer:1;            // Do not output buzz for next button event
      u16 LowBattery:1;            // 1 = Battery is low
      u16 UseMetricUnits:1;       // 1 = Use metric units, 0 = use English units
      u16 DelayOver:1;             // 1 = Timer delay over
   } States;
   u16 Flags;                       // Shortcut to all display flags (for reset)
} SystemInfo;

extern volatile SystemInfo System;

typedef struct SystemDataInfo_t
{
    // 4 byte device address overrides end device address set in "smpl_config.dat"
    unsigned char EndDeviceAddress[4];
} SystemDataInfo;

extern SystemDataInfo SystemData;

typedef union
{
   struct
   {
      u16 MeasureVoltage:1;        // 1 = Measure voltage
      u16 MeasureAcceleration:1;   // 1 = Measure acceleration
      u16 Buzzer:1;                // 1 = Output buzzer
   } States;
   u16 Flags;                           // Shortcut to all display flags (for reset)
} RequestInfo;

extern volatile RequestInfo SystemRequests;

#endif
