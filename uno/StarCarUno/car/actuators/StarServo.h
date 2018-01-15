//--------------------------------------------------------------------------------------------------
// <copyright file="StarServo.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#pragma once
#include "../StarCar.h"
#include <Servo.h>


enum StarServoResult
{
    ServoResult_Success = 0,
    ServoResult_Failed = -1,
};

class StarServo : public StarCarModule
{
    // ---------- Private fields ----------
private:
    int_t currentMicroseconds;
    pin_t stepPin;
    Servo servo;
    byte_t testStepIndex;

    // ---------- Public constructors ----------
public:
    StarServo(pin_t stepPin);

    // ---------- Public properties ----------
public:
    virtual const char* getName();

    // ---------- Public methods ----------
public:
    void Task(StarCar* car);

    void Test01();
    void Test02();
    void Test03();

    // ---------- Protected methods ----------
protected:
    virtual byte_t InitCore();
};
