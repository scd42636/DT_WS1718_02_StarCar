//--------------------------------------------------------------------------------------------------
// <copyright file="StarSonic.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Simone Huber</author>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "StarSonic.h"


// ---------- Public constructors ----------

StarSonic::StarSonic(pin_t signalPin, StarSonicLocation location)
{
    this->signalPin = signalPin;
    this->lastRequestTime = 0;
    this->location = location;
}

// ---------- Public properties ----------

StarSonicLocation StarSonic::getLocation()
{
    return this->location;
}

const char* StarSonic::getName()
{
    if (this->location == StarSonicLocation::SonicLocation_Back)
        return "Sonic (back)";

    return "Sonic (front)";
}

// ---------- Public methods ----------

void StarSonic::Task(StarCar* car)
{
    #if _DEBUG
    Serial.println("--> StarSonic::Task()");
    #endif

    ulong_t currentRequestTime = millis();
    ulong_t duration = currentRequestTime - this->lastRequestTime;

    if (this->signalPin != PIN_DISCONNECTED
        && (this->lastRequestTime == 0 || duration >= 100)
        && car->IsRequested(StarCarSensorRequest::CarSensorRequest_Sonic)) {
        //Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
        pinMode(this->signalPin, OUTPUT);

        digitalWrite(this->signalPin, LOW);
        delayMicroseconds(2);

        digitalWrite(this->signalPin, HIGH);
        delayMicroseconds(5);

        digitalWrite(this->signalPin, LOW);

        //Same pin is ued to read and receive
        pinMode(this->signalPin, INPUT);

        long_t duration = pulseIn(this->signalPin, HIGH);
        long_t distance = (long_t)((float_t)duration / 29.0 / 2.0); // seconds to cm's

        if (distance == 0)
            distance = millis() / 1000;

        if (this->location == StarSonicLocation::SonicLocation_Back)
            car->setDistanceBack(distance);
        else if (this->location == StarSonicLocation::SonicLocation_Front)
            car->setDistanceFront(distance);

        #if TEST
        Serial.print(this->getName());
        Serial.print(":");

        Serial.print(" Distance = ");
        Serial.println((long)distance);
        #endif

        this->lastRequestTime = currentRequestTime;
    }
}

// ---------- Protected methods ----------

byte_t StarSonic::InitCore()
{
    return StarSonicResult::SonicResult_Success;
}
