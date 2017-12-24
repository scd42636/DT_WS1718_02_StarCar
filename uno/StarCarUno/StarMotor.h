//--------------------------------------------------------------------------------------------------
// <copyright file="StarMotor.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

// Implemented using the protocol documented on:
// - https://www.pololu.com/docs/0J44/6.2.1

#pragma once
#include "StarCar.h"


enum StarMotorDirection
{
    MotorDirection_Forward = 0x85,
    MotorDirection_Backward = 0x86
};

enum StarMotorLimit
{
    MotorLimit_MaxSpeed = 0,
    MotorLimit_MaxAcceleration = 1,
    MotorLimit_MaxDeceleration = 2,
    MotorLimit_BrakeDuration = 3,

    MotorLimit_MaxSpeedForward = MotorLimit_MaxSpeed + 4,
    MotorLimit_MaxAccelerationForward = MotorLimit_MaxAcceleration + 4,
    MotorLimit_MaxDecelerationForward = MotorLimit_MaxDeceleration + 4,
    MotorLimit_BrakeDurationForward = MotorLimit_BrakeDuration + 4,

    MotorLimit_MaxSpeedBackward = MotorLimit_MaxSpeed + 8,
    MotorLimit_MaxAccelerationBackward = MotorLimit_MaxAcceleration + 8,
    MotorLimit_MaxDecelerationBackward = MotorLimit_MaxDeceleration + 8,
    MotorLimit_BrakeDurationBackward = MotorLimit_BrakeDuration + 8,
};

enum StarMotorLimitResult
{
    MotorLimitResult_Success = 0,
    MotorLimitResult_UnableToSetForwardLimit = 1,
    MotorLimitResult_UnableToSetBackwardLimit = 2,
    MotorLimitResult_UnableToSetLimit = 3,
};

enum StarMotorResult
{
    MotorResult_Success = 0,

    MotorResult_Failed = -1,
    MotorResult_TimeoutExpired = -2,

    MotorResult_UnableToSetLimit = -3,
    MotorResult_UnableToSetForwardLimit = -4,
    MotorResult_UnableToSetBackwardLimit = -5,

    MotorResult_DirectionIsOutOfRange = -10,
    MotorResult_SpeedIsOutOfRange = -20
};

/// <summary>
/// See https://www.pololu.com/docs/0J44/6.4
/// </summary>
enum StarMotorVariable
{
    // Status Flag Registers
    MotorVariable_ErrorStatus = 0,
    MotorVariable_ErrorsOccured = 1,
    MotorVariable_SerialErrorsOccured = 2,
    MotorVariable_LimitStatus = 3,
    MotorVariable_ResetFlags = 127,

    // RC Channel Inputs
    MotorVariable_RC1UnlimitedRawValue = 4,
    MotorVariable_RC1RawValue = 5,
    MotorVariable_RC1ScaledValue = 6,
    MotorVariable_RC2UnlimitedRawValue = 8,
    MotorVariable_RC2RawValue = 9,
    MotorVariable_RC2ScaledValue = 10,

    // Analog Channel Inputs
    MotorVariable_AN1UnlimitedRawValue = 12,
    MotorVariable_AN1RawValue = 13,
    MotorVariable_AN1ScaledValue = 14,
    MotorVariable_AN2UnlimitedRawValue = 16,
    MotorVariable_AN2RawValue = 17,
    MotorVariable_AN2ScaledValue = 18,

    // Diagnostic Variables
    MotorVariable_TargetSpeed = 20,
    MotorVariable_Speed = 21,
    MotorVariable_BrakeAmount = 22,
    MotorVariable_InputVoltage = 23,
    MotorVariable_Temperature = 24,
    MotorVariable_RCPeriod = 26,
    MotorVariable_BaudRateRegister = 27,
    MotorVariable_SystemTimeLow = 28,
    MotorVariable_SystemTimeHigh = 29,

    // Temporary Motor Limits
    MotorVariable_MaxSpeedForward = 30,
    MotorVariable_MaxAccelerationForward = 31,
    MotorVariable_MaxDecelerationForward = 32,
    MotorVariable_BrakeDurationForward = 33,
    MotorVariable_MaxSpeedReverse = 36,
    MotorVariable_MaxAccelerationReverse = 37,
    MotorVariable_MaxDecelerationReverse = 38,
    MotorVariable_BrakeDurationReverse = 39,
};

class StarMotor
{
    // ---------- Public const fields ----------
public:
    const int MinSpeed = 0;
    const int MaxSpeed = 3200;

    // ---------- Private fields ----------
private:
    int16_t currentSpeed;
    Pin errorPin;
    Pin receivePin;
    Pin resetPin;
    Pin transmitPin;
    int testStepIndex;

    SoftwareSerial serial = SoftwareSerial(0, 0);

    // ---------- Public constructors ----------
public:
    StarMotor(
        Pin receivePin,
        Pin transmitPin,
        Pin resetPin,
        Pin errorPin = PIN_DISCONNECTED);

    // ---------- Public methods ----------
public:
    StarMotorResult ChangeSpeed(int speed);
    StarMotorResult ChangeSpeed(int speed, StarMotorDirection direction);

    StarMotorResult ChangeLimit(StarMotorLimit limit, unsigned int value);
    StarMotorResult Init();

    int ReadCurrentSpeed();
    int ReadTargetSpeed();

    StarMotorResult ReadVariable(StarMotorVariable variable, unsigned int* value);

    void Stop();

    void Task(StarCar* car);
    void Test01();
    void Test02();

    // ---------- Private methods ----------
private:
    void ExitSafeStart();
};
