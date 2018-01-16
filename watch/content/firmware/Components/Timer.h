// -------------------------------------------------------------------------------------------------
// <copyright file="Timer.h" company="OTH Regensburg">
//     Copyright (c) OTH Regensburg, Algorithmen für Sensornetze. All rights reserved.
// </copyright>
// <autor>Dominik Scharnagl (MatNr. 3054541)</autor>
// -------------------------------------------------------------------------------------------------

#ifndef TIMER_H
#define TIMER_H

#include "Defines.h"
#include "BatteryDefines.h"
#include "SystemDefines.h"
#include "TimerDefines.h"


// ---------- Public methods (public predefines) ----------

void Timer_Delay(u16 ticks);

void Timer_Start();
void Timer_Stop();

void Timer_StartPeriodic(u16 ticks, ActionFunction callback);
void Timer_StopPeriodic();

void Timer_Setup();

#endif
