// -------------------------------------------------------------------------------------------------
// <copyright file="Buzzer.h" company="OTH Regensburg">
//     Copyright (c) OTH Regensburg, Algorithmen für Sensornetze. All rights reserved.
// </copyright>
// <autor>Dominik Scharnagl (MatNr. 3054541)</autor>
// -------------------------------------------------------------------------------------------------

#ifndef BUZZER_H
#define BUZZER_H

#include "Defines.h"
#include "BuzzerDefines.h"
#include "TimerDefines.h"


// ---------- Public methods (public predefines) ----------

void Buzzer_Countdown();
bool Buzzer_IsActive();

void Buzzer_Reset();
void Buzzer_Setup();

void Buzzer_Start(u8 cycles, u16 on_time, u16 off_time);
void Buzzer_Stop();

void Buzzer_Toggle();

#endif
