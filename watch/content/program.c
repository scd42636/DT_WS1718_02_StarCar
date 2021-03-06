// -------------------------------------------------------------------------------------------------
// <copyright file="Program.c" company="OTH Regensburg">
//     Copyright (c) OTH Regensburg, Algorithmen f�r Sensornetze. All rights reserved.
// </copyright>
// <autor>Dominik Scharnagl (MatNr. 3054541)</autor>
// -------------------------------------------------------------------------------------------------

#include "Firmware.h"


int main()
{
    // Initialize MCU and prepare firmware.
    Firmware_Setup();

    // Run firmware loop until an unexpected error occurs.
    while (Firmware_Loop());

    // Shut down firmware and try to exit as clean as possible.
    Firmware_Exit();
}
