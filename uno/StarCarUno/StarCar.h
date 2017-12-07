//--------------------------------------------------------------------------------------------------
// <copyright file="StarCar.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------


#pragma once
#include <Arduino.h>

// Implemented using the code provided on:
// - https://github.com/laurb9/StepperDriver/blob/master/src/BasicStepperDriver.h
static inline void delayMicros(unsigned long delay_us, unsigned long start_us = 0)
{
    if (delay_us) {
        if (!start_us) {
            start_us = micros();
        }

        if (delay_us > 50 /* MIN_YIELD_MICROS */) {
            yield();
        }

        // See https://www.gammon.com.au/millis
        while (micros() - start_us < delay_us);
    }
}