// -------------------------------------------------------------------------------------------------
// <copyright file="MotionControl.c" company="OTH Regensburg">
//     Copyright (c) OTH Regensburg, Algorithmen für Sensornetze. All rights reserved.
// </copyright>
// <autor>Dominik Scharnagl (MatNr. 3054541)</autor>
// -------------------------------------------------------------------------------------------------

#include "MotionControl.h"
#include "rfsimpliciti.h"


MotionControlInfo MotionControl;


// ---------- Private methods (private predefines) ----------

void HandleButtonInterruptReceived();


// ---------- Public methods (public defines) ----------

// Original: reset_rf()
void MotionControl_Reset()
{
    MotionControl.Command = MotionControlCommand_None;
    MotionControl.State = MotionControlState_Stopped;
    MotionControl.Mode = MotionControlMode_None;
}

void MotionControl_Setup()
{
    ButtonInterruptReceived = &HandleButtonInterruptReceived;
    MotionControl_Reset();

    // start_buzzer(1, ConvertMillisecondsToTicks(20), ConvertMillisecondsToTicks(150));
}

// ---------- Private methods (private defines) ----------

void HandleButtonInterruptReceived()
{
    u8 int_flag = BUTTONS_IFG & BUTTONS_IE;

    if (MotionControl.State == MotionControlState_Started)
    {
        if ((int_flag & BUTTON_NUM_PIN) == BUTTON_NUM_PIN)
        {
            simpliciti_flag |= SIMPLICITI_TRIGGER_STOP;
        }
    }
}
