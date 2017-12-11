//--------------------------------------------------------------------------------------------------
// <copyright file="StarServo.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "StarServo.h"


// ---------- Public constructors ----------

StarServo::StarServo(short stepPin)
{
    this->stepPin = stepPin;
}

// ---------- Public methods ----------

StarServoResult StarServo::Init()
{
    pinMode(this->stepPin, OUTPUT);
    this->servo.attach(this->stepPin);

    // Move into neutral (= straight) direction.
    this->servo.writeMicroseconds(1365);

    return StarServoResult::SR_Success;
}

void StarServo::Task()
{
    #if _DEBUG
    Serial.println("--> StarServo::Task()");
    #endif
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
