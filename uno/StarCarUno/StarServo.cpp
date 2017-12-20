//--------------------------------------------------------------------------------------------------
// <copyright file="StarServo.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "StarServo.h"
#define SERVO_LEFT_MS 1100
#define SERVO_CENTER_MS 1365
#define SERVO_RIGHT_MS 1600


// ---------- Public constructors ----------

StarServo::StarServo(short stepPin)
{
    this->stepPin = stepPin;
    this->currentMicroseconds = SERVO_CENTER_MS;

    this->testStepIndex = 0;
}

// ---------- Public methods ----------

StarServoResult StarServo::Init()
{
    pinMode(this->stepPin, OUTPUT);
    this->servo.attach(this->stepPin);

    // Move into neutral (= straight) direction.
    this->servo.writeMicroseconds(SERVO_CENTER_MS);

    return StarServoResult::SR_Success;
}

void StarServo::Task(StarCar* car)
{
    #if _DEBUG
    Serial.println("--> StarServo::Task()");
    #endif

    int16_t ms = SERVO_CENTER_MS;

    if (car->getMode() == StarCarMode::CM_None) {
        int8_t dir = car->getDirection();
        float direction = (float)dir / 100;

        if (direction < 0) {
            // Results into values between 1100 and 1365 (inclusive).
            ms = SERVO_CENTER_MS - (float)(SERVO_CENTER_MS - SERVO_LEFT_MS) * ((-1) * direction);

            //if (ms > SERVO_CENTER_MS - 20)
            //    ms = SERVO_CENTER_MS;
        }
        else {
            ms = SERVO_CENTER_MS + (float)(SERVO_RIGHT_MS - SERVO_CENTER_MS) * direction;

            //if (ms < SERVO_CENTER_MS + 20)
            //    ms = SERVO_CENTER_MS;
        }
    }

    if (this->currentMicroseconds != ms) {
        #if TEST
        Serial.print("Servo: dir = ");
        Serial.println(dir);

        Serial.print("Servo: ms = ");
        Serial.println(ms);
        #else
        this->servo.writeMicroseconds(ms);
        #endif

        this->currentMicroseconds = ms;
        Serial.print("x");
    }
}

void StarServo::Test01()
{
    int pos = 0;

    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees in steps of 1 degree
        this->servo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
    }

    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
        this->servo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
    }
}

void StarServo::Test02()
{
    // ms = 1100 --> ganz links
    // ms = 1400 --> Mitte
    // ms = 1600 --> ganz rechts

    int ms = 1100;

    for (int index = 0; index < 60; index++) {
        ms += 10;

        Serial.print("ms = ");
        Serial.println(ms);

        this->servo.writeMicroseconds(ms);
        delay(50);
    }

    for (int index = 0; index < 60; index++) {
        ms -= 10;

        Serial.print("ms = ");
        Serial.println(ms);

        this->servo.writeMicroseconds(ms);
        delay(50);
    }
}

void StarServo::Test03()
{
    if (this->testStepIndex > 60)
        this->testStepIndex = 0;

    int ms = 1100 + (this->testStepIndex++) * 10;

    Serial.print("ms = ");
    Serial.println(ms);

    this->servo.writeMicroseconds(ms);
    delay(50);
}

