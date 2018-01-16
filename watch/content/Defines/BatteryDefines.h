// -------------------------------------------------------------------------------------------------
// <copyright file="BatteryDefines.h" company="OTH Regensburg">
//     Copyright (c) OTH Regensburg, Algorithmen für Sensornetze. All rights reserved.
// </copyright>
// <autor>Dominik Scharnagl (MatNr. 3054541)</autor>
// -------------------------------------------------------------------------------------------------

#ifndef BATTERYDEFINES_H
#define BATTERYDEFINES_H

#include "Defines.h"


// Battery high voltage threshold
#define BATTERY_HIGH_THRESHOLD                  (360u)

// Battery end of life voltage threshold -> disable radio, show "lobatt" message
#define BATTERY_LOW_THRESHOLD                   (240u)

// Show "lobatt" message every n seconds
#define BATTERY_LOW_MESSAGE_CYCLE               (15u)


typedef struct BatteryInfo_t
{
    s16 Offset;                 // Battery voltage offset
    u16 Voltage;                // Battery voltage
} BatteryInfo;

extern BatteryInfo SystemBattery;

#endif
