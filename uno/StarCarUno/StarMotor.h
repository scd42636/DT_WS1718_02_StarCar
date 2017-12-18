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
    MD_Forward = 0x85,
    MD_Backward = 0x86
};

enum StarMotorLimit
{
    ML_MaxSpeed = 0,
    ML_MaxAcceleration = 1,
    ML_MaxDeceleration = 2,
    ML_BrakeDuration = 3,

    ML_MaxSpeedForward = ML_MaxSpeed + 4,
    ML_MaxAccelerationForward = ML_MaxAcceleration + 4,
    ML_MaxDecelerationForward = ML_MaxDeceleration + 4,
    ML_BrakeDurationForward = ML_BrakeDuration + 4,

    ML_MaxSpeedBackward = ML_MaxSpeed + 8,
    ML_MaxAccelerationBackward = ML_MaxAcceleration + 8,
    ML_MaxDecelerationBackward = ML_MaxDeceleration + 8,
    ML_BrakeDurationBackward = ML_BrakeDuration + 8,
};

enum StarMotorLimitResult
{
    MLR_Success = 0,
    MLR_UnableToSetForwardLimit = 1,
    MLR_UnableToSetBackwardLimit = 2,
    MLR_UnableToSetLimit = 3,
};

enum StarMotorResult
{
    MR_Success = 0,

    MR_Failed = -1,
    MR_TimeoutExpired = -2,

    MR_UnableToSetLimit = -3,
    MR_UnableToSetForwardLimit = -4,
    MR_UnableToSetBackwardLimit = -5,

    MR_DirectionIsOutOfRange = -10,
    MR_SpeedIsOutOfRange = -20
};

/// <summary>
/// See https://www.pololu.com/docs/0J44/6.4
/// </summary>
enum StarMotorVariable
{
    // Status Flag Registers
    MV_ErrorStatus = 0,
    MV_ErrorsOccured = 1,
    MV_SerialErrorsOccured = 2,
    MV_LimitStatus = 3,
    MV_ResetFlags = 127,

    // RC Channel Inputs
    MV_RC1UnlimitedRawValue = 4,
    MV_RC1RawValue = 5,
    MV_RC1ScaledValue = 6,
    MV_RC2UnlimitedRawValue = 8,
    MV_RC2RawValue = 9,
    MV_RC2ScaledValue = 10,

    // Analog Channel Inputs
    MV_AN1UnlimitedRawValue = 12,
    MV_AN1RawValue = 13,
    MV_AN1ScaledValue = 14,
    MV_AN2UnlimitedRawValue = 16,
    MV_AN2RawValue = 17,
    MV_AN2ScaledValue = 18,

    // Diagnostic Variables
    MV_TargetSpeed = 20,
    MV_Speed = 21,
    MV_BrakeAmount = 22,
    MV_InputVoltage = 23,
    MV_Temperature = 24,
    MV_RCPeriod = 26,
    MV_BaudRateRegister = 27,
    MV_SystemTimeLow = 28,
    MV_SystemTimeHigh = 29,

    // Temporary Motor Limits
    MV_MaxSpeedForward = 30,
    MV_MaxAccelerationForward = 31,
    MV_MaxDecelerationForward = 32,
    MV_BrakeDurationForward = 33,
    MV_MaxSpeedReverse = 36,
    MV_MaxAccelerationReverse = 37,
    MV_MaxDecelerationReverse = 38,
    MV_BrakeDurationReverse = 39,
};

class StarMotor
{
    // ---------- Public const fields ----------
public:
    const int MinSpeed = 0;
    const int MaxSpeed = 3200;

    // ---------- Private fields ----------
private:
    short errorPin;
    short receivePin;
    short resetPin;
    short transmitPin;
    int testStepIndex;

    SoftwareSerial serial = SoftwareSerial(0, 0);

    // ---------- Public constructors ----------
public:
    StarMotor(
        short receivePin,
        short transmitPin,
        short resetPin,
        short errorPin = PIN_DISCONNECTED);

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
