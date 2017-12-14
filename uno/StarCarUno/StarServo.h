//--------------------------------------------------------------------------------------------------
// <copyright file="StarServo.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#pragma once
#include "StarCar.h"
#include <Servo.h>;


enum StarServoResult
{
    SR_Success = 0,
    SR_Failed = -1,
};

class StarServo
{
    // ---------- Private fields ----------
private:
    short stepPin;
    Servo servo;
    int16_t currentMicroseconds;

    // ---------- Public constructors ----------
public:
    StarServo(short stepPin);

    // ---------- Public methods ----------
public:
    StarServoResult Init();
    void Task(StarCar* car);

    void Test01();
    void Test02();
};
