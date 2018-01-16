// -------------------------------------------------------------------------------------------------
// <copyright file="Accelerator.h" company="OTH Regensburg">
//     Copyright (c) OTH Regensburg, Algorithmen für Sensornetze. All rights reserved.
// </copyright>
// <autor>Dominik Scharnagl (MatNr. 3054541)</autor>
// -------------------------------------------------------------------------------------------------

#ifndef ACCELERATOR_H
#define ACCELERATOR_H

#include "Defines.h"
#include "AcceleratorDefines.h"

// =================================================================================================
// CMA3000-D0x acceleration sensor configuration
// =================================================================================================
// DCO frequency division factor determining speed of the acceleration sensor SPI interface
// Speed in Hz = 12MHz / AS_BR_DIVIDER (max. 500kHz)
#define CMA_AS_BR_DIVIDER    (30u)

// Acceleration measurement range in g
// Valid ranges are: 2 and 8
#define CMA_AS_RANGE         (2u)

// Sample rate for acceleration values in Hz
// Valid sample rates for 2g range are:     100, 400
// Valid sample rates for 8g range are: 40, 100, 400
#define CMA_AS_SAMPLE_RATE   (100u)

#include "Timer.h"


// ---------- Public methods (public predefines) ----------

void Accelerator_GetData(u8* data);
bool Accelerator_IsActive();

void Accelerator_Setup();

void Accelerator_Start();
void Accelerator_Stop();

#endif
