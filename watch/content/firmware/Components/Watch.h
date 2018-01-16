// -------------------------------------------------------------------------------------------------
// <copyright file="Watch.h" company="OTH Regensburg">
//     Copyright (c) OTH Regensburg, Algorithmen für Sensornetze. All rights reserved.
// </copyright>
// <autor>Dominik Scharnagl (MatNr. 3054541)</autor>
// -------------------------------------------------------------------------------------------------

#ifndef WATCH_H
#define WATCH_H

#include "Defines.h"

#include "AcceleratorDefines.h"
#include "BatteryDefines.h"
#include "ButtonDefines.h"
#include "BuzzerDefines.h"
#include "SystemDefines.h"

#include "display.h"
#include "Timer.h"


// ---------- Public fields ----------

extern void (*ButtonInterruptReceived)();


// ---------- Public methods (public predefines) ----------

void Watch_GoIntoIdleMode();
void Watch_GoIntoLowPowerMode();

void Watch_Setup();
void Watch_ShowWelcome();

#endif
