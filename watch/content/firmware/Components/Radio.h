// -------------------------------------------------------------------------------------------------
// <copyright file="Radio.h" company="OTH Regensburg">
//     Copyright (c) OTH Regensburg, Algorithmen für Sensornetze. All rights reserved.
// </copyright>
// <autor>Dominik Scharnagl (MatNr. 3054541)</autor>
// -------------------------------------------------------------------------------------------------

#ifndef RADIO_H
#define RADIO_H

#include "Defines.h"
#include "rf1a.h"

//#include "mrfi_radio.h"
extern void MRFI_RadioIsr();


// ---------- Public methods (public predefines) ----------

bool Radio_IsOpen();

void Radio_Close();
void Radio_Open();
void Radio_Powerdown();
void Radio_Reset();
void Radio_Setup();

#endif
