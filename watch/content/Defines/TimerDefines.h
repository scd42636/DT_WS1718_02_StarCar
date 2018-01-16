// -------------------------------------------------------------------------------------------------
// <copyright file="TimerDefines.h" company="OTH Regensburg">
//     Copyright (c) OTH Regensburg, Algorithmen für Sensornetze. All rights reserved.
// </copyright>
// <autor>Dominik Scharnagl (MatNr. 3054541)</autor>
// -------------------------------------------------------------------------------------------------

#ifndef TIMERDEFINES_H
#define TIMERDEFINES_H

#include "Defines.h"


typedef struct TimerInfo_t
{
    // Timer0_A3 periodic delay
    u16 Ticks;
} TimerInfo;

extern TimerInfo SystemTimer;

#endif
