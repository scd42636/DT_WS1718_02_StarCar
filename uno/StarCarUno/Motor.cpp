//--------------------------------------------------------------------------------------------------
// <copyright file="Motor.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "Motor.h"
#include <Arduino.h>

// ---------- Public constructors ----------

Motor::Motor(
    short receivePin,
    short transmitPin,
    short resetPin,
    short errorPin)
{
    this->receivePin = receivePin;
    this->transmitPin = transmitPin;
    this->resetPin = resetPin;
    this->errorPin = errorPin;

    this->serial = SoftwareSerial(this->receivePin, this->transmitPin);
}

// ---------- Public methods ----------

MotorResult Motor::ChangeSpeed(int speed)
{
    if (speed < 0)
        return this->ChangeSpeed(-speed, MotorDirection::MD_Backward);

    return this->ChangeSpeed(speed, MotorDirection::MD_Forward);
}

MotorResult Motor::ChangeSpeed(int speed, MotorDirection direction)
{
    if (direction != MotorDirection::MD_Forward && direction != MotorDirection::MD_Backward)
        return MotorResult::MR_DirectionIsOutOfRange;

    if (speed < MinSpeed || speed > MaxSpeed)
        return MotorResult::MR_SpeedIsOutOfRange;

    this->serial.write(direction);

    this->serial.write(speed & 0x1F);   // low five bits
    this->serial.write(speed >> 5);     // high seven bits

    return MotorResult::MR_Success;
}

MotorResult Motor::ChangeLimit(MotorLimit limit, unsigned int value)
{
    this->serial.write(0xA2);
    this->serial.write(limit);
    this->serial.write(value & 0x7F);
    this->serial.write(value >> 7);

    char result = 0;

    if (this->serial.readBytes(&result, 1) == 0)
        return MotorResult::MR_TimeoutExpired;

    if (result == MotorLimitResult::MLR_UnableToSetLimit)
        return MotorResult::MR_UnableToSetLimit;

    if (result == MotorLimitResult::MLR_UnableToSetForwardLimit)
        return MotorResult::MR_UnableToSetForwardLimit;

    if (result == MotorLimitResult::MLR_UnableToSetBackwardLimit)
        return MotorResult::MR_UnableToSetBackwardLimit;

    if (result == MotorLimitResult::MLR_Success) {
        this->ExitSafeStart();
        return MotorResult::MR_Success;
    }

    return MotorResult::MR_Failed;
}

int Motor::ReadCurrentSpeed()
{
    unsigned int value = 0;

    if (this->ReadVariable(MotorVariable::MV_Speed, &value) == MotorResult::MR_Success)
        return value;

    return -1;
}

int Motor::ReadTargetSpeed()
{
    unsigned int value = 0;

    if (this->ReadVariable(MotorVariable::MV_TargetSpeed, &value) == MotorResult::MR_Success)
        return value;

    return -1;
}

MotorResult Motor::ReadVariable(MotorVariable variable, unsigned int* value)
{
    *value = 0;

    this->serial.write(0xA1);
    this->serial.write(variable);

    char lowByte = 0;

    if (this->serial.readBytes(&lowByte, 1) == 0)
        return MotorResult::MR_TimeoutExpired;

    char highByte = 0;

    if (this->serial.readBytes(&highByte, 1) == 0)
        return MotorResult::MR_TimeoutExpired;

    *value = lowByte + 256 * highByte;

    // See https://www.pololu.com/docs/0J44/6.2.1, Section: Get Variable (any input mode)
    // If the variable type is signed and the above result is greater than 32767, you will need to
    // subtract 65536 from the result to obtain the correct, signed value. Alternatively, if it is
    // supported by the language you are using, you can cast the result to a signed 16-bit
    // data type.
    if (*value > 32767
        && variable == MotorVariable::MV_RC1ScaledValue
        || variable == MotorVariable::MV_RC2ScaledValue
        || variable == MotorVariable::MV_AN1ScaledValue
        || variable == MotorVariable::MV_AN2ScaledValue
        || variable == MotorVariable::MV_TargetSpeed
        || variable == MotorVariable::MV_Speed) {
        *value -= 65536;
    }

    return MotorResult::MR_Success;
}

MotorResult Motor::Setup()
{
    this->serial.begin(19200);

    // Briefly reset SMC when Arduino starts up (optional).
    pinMode(this->resetPin, OUTPUT);

    digitalWrite(this->resetPin, LOW);  // reset SMC
    delay(1);                           // wait 1 ms

    pinMode(this->resetPin, INPUT);     // let SMC run again

    // Must wait at least 1 ms after reset before transmitting.
    delay(5);

    // This lets us read the state of the SMC ERR pin (optional).
    pinMode(this->errorPin, INPUT);

    // Send baud-indicator byte.
    this->serial.write(0xAA);

    this->ExitSafeStart();
    return MotorResult::MR_Success;
}

void Motor::Stop()
{
    this->ExitSafeStart();
}

// ---------- Private methods ----------

void Motor::ExitSafeStart()
{
    // Clear the safe-start violation.
    this->serial.write(0x83);
}