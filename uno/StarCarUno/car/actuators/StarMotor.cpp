//--------------------------------------------------------------------------------------------------
// <copyright file="StarMotor.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "StarMotor.h"


// ---------- Public constructors ----------

StarMotor::StarMotor(
    pin_t receivePin,
    pin_t transmitPin,
    pin_t resetPin,
    pin_t errorPin)
{
    this->receivePin = receivePin;
    this->transmitPin = transmitPin;
    this->resetPin = resetPin;
    this->errorPin = errorPin;

    this->serial = SoftwareSerial(this->receivePin, this->transmitPin);

    this->currentSpeed = 0;
    this->testStepIndex = 0;
}

// ---------- Public properties ----------

const char* StarMotor::getName()
{
    return "Motor";
}

// ---------- Public methods ----------

StarMotorResult StarMotor::ChangeSpeed(short_t speed)
{
    if (speed < 0)
        return this->ChangeSpeed(-speed, StarMotorDirection::MotorDirection_Backward);

    return this->ChangeSpeed(speed, StarMotorDirection::MotorDirection_Forward);
}

StarMotorResult StarMotor::ChangeSpeed(short_t speed, StarMotorDirection direction)
{
    if (direction != StarMotorDirection::MotorDirection_Forward && direction != StarMotorDirection::MotorDirection_Backward)
        return StarMotorResult::MotorResult_DirectionIsOutOfRange;

    if (speed < MinSpeed || speed > MaxSpeed)
        return StarMotorResult::MotorResult_SpeedIsOutOfRange;

    this->serial.write(direction);

    this->serial.write(speed & 0x1F);   // low five bits
    this->serial.write(speed >> 5);     // high seven bits

    this->isStopped = false;
    return StarMotorResult::MotorResult_Success;
}

StarMotorResult StarMotor::ChangeLimit(StarMotorLimit limit, ushort_t value)
{
    this->serial.write(0xA2);
    this->serial.write(limit);
    this->serial.write(value & 0x7F);
    this->serial.write(value >> 7);

    char result = 0;

    if (this->serial.readBytes(&result, 1) == 0)
        return StarMotorResult::MotorResult_TimeoutExpired;

    if (result == StarMotorLimitResult::MotorLimitResult_UnableToSetLimit)
        return StarMotorResult::MotorResult_UnableToSetLimit;

    if (result == StarMotorLimitResult::MotorLimitResult_UnableToSetForwardLimit)
        return StarMotorResult::MotorResult_UnableToSetForwardLimit;

    if (result == StarMotorLimitResult::MotorLimitResult_UnableToSetBackwardLimit)
        return StarMotorResult::MotorResult_UnableToSetBackwardLimit;

    if (result == StarMotorLimitResult::MotorLimitResult_Success) {
        this->ExitSafeStart();
        return StarMotorResult::MotorResult_Success;
    }

    return StarMotorResult::MotorResult_Failed;
}

short_t StarMotor::ReadCurrentSpeed()
{
    ushort_t value = 0;

    if (this->ReadVariable(StarMotorVariable::MotorVariable_Speed, &value) == StarMotorResult::MotorResult_Success)
        return value;

    return -1;
}

short_t StarMotor::ReadTargetSpeed()
{
    ushort_t value = 0;

    if (this->ReadVariable(StarMotorVariable::MotorVariable_TargetSpeed, &value) == StarMotorResult::MotorResult_Success)
        return value;

    return -1;
}

StarMotorResult StarMotor::ReadVariable(StarMotorVariable variable, ushort_t* value)
{
    *value = 0;

    this->serial.write(0xA1);
    this->serial.write(variable);

    char lowByte = 0;

    if (this->serial.readBytes(&lowByte, 1) == 0)
        return StarMotorResult::MotorResult_TimeoutExpired;

    char highByte = 0;

    if (this->serial.readBytes(&highByte, 1) == 0)
        return StarMotorResult::MotorResult_TimeoutExpired;

    *value = lowByte + 256 * highByte;

    // See https://www.pololu.com/docs/0J44/6.2.1, Section: Get Variable (any input mode)
    // If the variable type is signed and the above result is greater than 32767, you will need to
    // subtract 65536 from the result to obtain the correct, signed value. Alternatively, if it is
    // supported by the language you are using, you can cast the result to a signed 16-bit
    // data type.
    if (*value > 32767
        && variable == StarMotorVariable::MotorVariable_RC1ScaledValue
        || variable == StarMotorVariable::MotorVariable_RC2ScaledValue
        || variable == StarMotorVariable::MotorVariable_AN1ScaledValue
        || variable == StarMotorVariable::MotorVariable_AN2ScaledValue
        || variable == StarMotorVariable::MotorVariable_TargetSpeed
        || variable == StarMotorVariable::MotorVariable_Speed) {
        *value -= 65536;
    }

    return StarMotorResult::MotorResult_Success;
}

void StarMotor::Stop()
{
    if (!this->isStopped) {
        this->ExitSafeStart();
        this->isStopped = true;
    }
}

void StarMotor::Task(StarCar* car)
{
    #if _DEBUG
    Serial.println("--> StarMotor::Task()");
    #endif

    if (car->getEngineMode() == StarCarEngineMode::CarEngineMode_On) {
        sbyte_t acceleration = car->getSpeed();
        short_t speed = (-1) * ((float_t)1000 * ((float_t)acceleration / 100));

        if (this->currentSpeed != speed) {
            #if TEST
            Serial.print("Motor: accel = ");
            Serial.println(acceleration);

            Serial.print("Motor: speed = ");
            Serial.println(speed);
            #else
            this->ChangeSpeed(speed);
            #endif

            this->currentSpeed = speed;
        }
    }
    else if (car->getEngineMode() == StarCarEngineMode::CarEngineMode_Off) {
        this->Stop();
        this->currentSpeed = 0;
    }
}

void StarMotor::Test01()
{
    //this->ChangeSpeed(300);
    int_t speed = 0;

    for (int_t index = 0; index < 320; index++) {
        speed += 10;

        Serial.print("Speed = ");
        Serial.println(speed);

        this->ChangeSpeed(speed);
        delay(500);
    }

    short_t currentSpeed = this->ReadCurrentSpeed();
    short_t targetSpeed = this->ReadTargetSpeed();

    Serial.print("Target Speed: ");
    Serial.println(targetSpeed);

    Serial.print("Current Speed: ");
    Serial.println(currentSpeed);

    Serial.println("-----");
}

void StarMotor::Test02()
{
    if (this->testStepIndex > 100)
        this->testStepIndex = 0;

    short_t speed = (this->testStepIndex++) * 10;

    Serial.print("Speed = ");
    Serial.println(speed);

    this->ChangeSpeed(speed);
    Serial.println("-----");
}

// ---------- Protected methods ----------

byte_t StarMotor::InitCore()
{
    this->serial.begin(19200);

    // Briefly reset SMC when Arduino starts up (optional).
    pinMode(this->resetPin, OUTPUT);

    digitalWrite(this->resetPin, LOW);  // reset SMC
    delay(1); // 10                           // wait 1 ms

                //pinMode(this->resetPin, INPUT);     // let SMC run again

                // Must wait at least 1 ms after reset before transmitting.
    delay(5); // 50

    if (this->errorPin != PIN_DISCONNECTED) {
        // This lets us read the state of the SMC ERR pin (optional).
        pinMode(this->errorPin, INPUT);
    }

    // Send baud-indicator byte.
    this->serial.write(0xAA);

    this->ChangeLimit(StarMotorLimit::MotorLimit_MaxAccelerationForward, 1);
    this->ChangeLimit(StarMotorLimit::MotorLimit_MaxAccelerationBackward, 1);
    this->ChangeLimit(StarMotorLimit::MotorLimit_MaxDeceleration, 10);

    this->ExitSafeStart();
    return StarMotorResult::MotorResult_Success;
}

// ---------- Private methods ----------

void StarMotor::ExitSafeStart()
{
    // Clear the safe-start violation.
    this->serial.write(0x83);
}
