// -------------------------------------------------------------------------------------------------
// <copyright file="Defines.h" company="OTH Regensburg">
//     Copyright (c) OTH Regensburg, Algorithmen für Sensornetze. All rights reserved.
// </copyright>
// <autor>Dominik Scharnagl (MatNr. 3054541)</autor>
// -------------------------------------------------------------------------------------------------

#ifndef DEFINES_H
#define DEFINES_H

#include <cc430x613x.h>
#include <bm.h>


#define true 1
#define false 0

#define nullptr NULL

typedef unsigned char bool;

typedef unsigned char byte;
typedef signed char sbyte;

#define HasFlag(flags, bit)               ((flags & bit) == bit)

#define ConvertMicrosecondsToTicks(microseconds) (((microseconds) * 32768) / 1000000)
#define ConvertMillisecondsToTicks(milliseconds) (((milliseconds) * 32768) / 1000)

typedef void (*ActionFunction)();
#define Action(name) void (*name)()

#endif
