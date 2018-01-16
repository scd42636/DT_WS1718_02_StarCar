// -------------------------------------------------------------------------------------------------
// <copyright file="MotionControl.h" company="OTH Regensburg">
//     Copyright (c) OTH Regensburg, Algorithmen für Sensornetze. All rights reserved.
// </copyright>
// <autor>Dominik Scharnagl (MatNr. 3054541)</autor>
// -------------------------------------------------------------------------------------------------

#ifndef MOTIONCONTROL_H
#define MOTIONCONTROL_H

#include "Defines.h"
#include "Radio.h"
#include "Watch.h"


typedef enum MotionControlMode_t
{
    MotionControlMode_None = 0,
    MotionControlMode_Direction = 1,
    MotionControlMode_Speed = 2
} MotionControlMode;

typedef enum MotionControlCommand_t
{
    MotionControlCommand_None = 0,
    MotionControlCommand_Start = 1,
    MotionControlCommand_Stop = 2
} MotionControlCommand;

typedef enum MotionControlState_t
{
    MotionControlState_Undefined = 0,
    MotionControlState_Stopped = 1,
    MotionControlState_Started = 2
} MotionControlState;

typedef struct MotionControlInfo_t
{
    MotionControlCommand Command;
    MotionControlMode Mode;
    MotionControlState State;
    u16 Timeout;
    u8 Value;
} MotionControlInfo;


extern MotionControlInfo MotionControl;


// ---------- Public methods (public predefines) ----------

void MotionControl_Setup();

#endif
