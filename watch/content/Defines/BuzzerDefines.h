// -------------------------------------------------------------------------------------------------
// <copyright file="BuzzerDefines.h" company="OTH Regensburg">
//     Copyright (c) OTH Regensburg, Algorithmen für Sensornetze. All rights reserved.
// </copyright>
// <autor>Dominik Scharnagl (MatNr. 3054541)</autor>
// -------------------------------------------------------------------------------------------------

#ifndef BUZZERDEFINES_H
#define BUZZERDEFINES_H

#include "Defines.h"


// Buzzer states
#define BUZZER_OFF                                                      (0u)
#define BUZZER_ON_OUTPUT_DISABLED                       (1u)
#define BUZZER_ON_OUTPUT_ENABLED                        (2u)

// Buzzer output signal frequency = 32,768kHz/(BUZZER_TIMER_STEPS+1)/2 = 2.7kHz
#define BUZZER_TIMER_STEPS                                      (5u)

// Buzzer on time
#define BUZZER_ON_TICKS                                         (ConvertMillisecondsToTicks(20))

// Buzzer off time
#define BUZZER_OFF_TICKS                                        (ConvertMillisecondsToTicks(200))


typedef struct BuzzerInfo_t
{
    // Keep output for "time" seconds
    u8 time;

    // On/off duty
    u16 on_time;
    u16 off_time;

    // Current buzzer output state
    u8 state;
} BuzzerInfo;

extern BuzzerInfo SystemBuzzer;

#endif
