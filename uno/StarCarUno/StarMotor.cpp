//--------------------------------------------------------------------------------------------------
// <copyright file="StarMotor.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "StarMotor.h"


// ---------- Public constructors ----------

StarMotor::StarMotor(
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

    this->currentSpeed = 0;
    this->testStepIndex = 0;
}

// ---------- Public methods ----------

StarMotorResult StarMotor::ChangeSpeed(int speed)
{
    if (speed < 0)
        return this->ChangeSpeed(-speed, StarMotorDirection::MD_Backward);

    return this->ChangeSpeed(speed, StarMotorDirection::MD_Forward);
}

StarMotorResult StarMotor::ChangeSpeed(int speed, StarMotorDirection direction)
{
    if (direction != StarMotorDirection::MD_Forward && direction != StarMotorDirection::MD_Backward)
        return StarMotorResult::MR_DirectionIsOutOfRange;

    if (speed < MinSpeed || speed > MaxSpeed)
        return StarMotorResult::MR_SpeedIsOutOfRange;

    this->serial.write(direction);

    this->serial.write(speed & 0x1F);   // low five bits
    this->serial.write(speed >> 5);     // high seven bits

    return StarMotorResult::MR_Success;
}

StarMotorResult StarMotor::ChangeLimit(StarMotorLimit limit, unsigned int value)
{
    this->serial.write(0xA2);
    this->serial.write(limit);
    this->serial.write(value & 0x7F);
    this->serial.write(value >> 7);

    char result = 0;

    if (this->serial.readBytes(&result, 1) == 0)
        return StarMotorResult::MR_TimeoutExpired;

    if (result == StarMotorLimitResult::MLR_UnableToSetLimit)
        return StarMotorResult::MR_UnableToSetLimit;

    if (result == StarMotorLimitResult::MLR_UnableToSetForwardLimit)
        return StarMotorResult::MR_UnableToSetForwardLimit;

    if (result == StarMotorLimitResult::MLR_UnableToSetBackwardLimit)
        return StarMotorResult::MR_UnableToSetBackwardLimit;

    if (result == StarMotorLimitResult::MLR_Success) {
        this->ExitSafeStart();
        return StarMotorResult::MR_Success;
    }

    return StarMotorResult::MR_Failed;
}

StarMotorResult StarMotor::Init()
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

    this->ChangeLimit(StarMotorLimit::ML_MaxAccelerationForward, 1);
    this->ChangeLimit(StarMotorLimit::ML_MaxAccelerationBackward, 1);
    this->ChangeLimit(StarMotorLimit::ML_MaxDeceleration, 10);

    this->ExitSafeStart();
    return StarMotorResult::MR_Success;
}

int StarMotor::ReadCurrentSpeed()
{
    unsigned int value = 0;

    if (this->ReadVariable(StarMotorVariable::MV_Speed, &value) == StarMotorResult::MR_Success)
        return value;

    return -1;
}

int StarMotor::ReadTargetSpeed()
{
    unsigned int value = 0;

    if (this->ReadVariable(StarMotorVariable::MV_TargetSpeed, &value) == StarMotorResult::MR_Success)
        return value;

    return -1;
}

StarMotorResult StarMotor::ReadVariable(StarMotorVariable variable, unsigned int* value)
{
    *value = 0;

    this->serial.write(0xA1);
    this->serial.write(variable);

    char lowByte = 0;

    if (this->serial.readBytes(&lowByte, 1) == 0)
        return StarMotorResult::MR_TimeoutExpired;

    char highByte = 0;

    if (this->serial.readBytes(&highByte, 1) == 0)
        return StarMotorResult::MR_TimeoutExpired;

    *value = lowByte + 256 * highByte;

    // See https://www.pololu.com/docs/0J44/6.2.1, Section: Get Variable (any input mode)
    // If the variable type is signed and the above result is greater than 32767, you will need to
    // subtract 65536 from the result to obtain the correct, signed value. Alternatively, if it is
    // supported by the language you are using, you can cast the result to a signed 16-bit
    // data type.
    if (*value > 32767
        && variable == StarMotorVariable::MV_RC1ScaledValue
        || variable == StarMotorVariable::MV_RC2ScaledValue
        || variable == StarMotorVariable::MV_AN1ScaledValue
        || variable == StarMotorVariable::MV_AN2ScaledValue
        || variable == StarMotorVariable::MV_TargetSpeed
        || variable == StarMotorVariable::MV_Speed) {
        *value -= 65536;
    }

    return StarMotorResult::MR_Success;
}

void StarMotor::Stop()
{
    this->ExitSafeStart();
}

void StarMotor::Task(StarCar* car)
{
    #if _DEBUG
    Serial.println("--> StarMotor::Task()");
    #endif

    if (car->getEngineMode() == StarCarEngineMode::CEM_On) {
        int8_t acceleration = car->getAcceleration();
        int16_t speed = (-1) * ((float)1000 * ((float)acceleration / 100));

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
    else if (car->getEngineMode() == StarCarEngineMode::CEM_Off) {
        this->Stop();
        this->currentSpeed = 0;
    }
}

void StarMotor::Test01()
{
    //this->ChangeSpeed(300);
    int speed = 0;

    for (int index = 0; index < 320; index++) {
        speed += 10;

        Serial.print("Speed = ");
        Serial.println(speed);

        this->ChangeSpeed(speed);
        delay(500);
    }

    int currentSpeed = this->ReadCurrentSpeed();
    int targetSpeed = this->ReadTargetSpeed();

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

    int speed = (this->testStepIndex++) * 10;

    Serial.print("Speed = ");
    Serial.println(speed);

    this->ChangeSpeed(speed);
    Serial.println("-----");
}

// ---------- Private methods ----------

void StarMotor::ExitSafeStart()
{
    // Clear the safe-start violation.
    this->serial.write(0x83);
}
