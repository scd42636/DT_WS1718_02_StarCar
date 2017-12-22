//--------------------------------------------------------------------------------------------------
// <copyright file="StarSonic.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Simone Huber</author>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "StarSonic.h"


// ---------- Public constructors ----------

StarSonic::StarSonic(Pin signalPin, StarSonicLocation location)
{
    this->signalPin = signalPin;
    this->location = location;
}

// ---------- Public properties ----------

StarSonicLocation StarSonic::getLocation()
{
    return this->location;
}

// ---------- Public methods ----------

StarSonicResult StarSonic::Init()
{
    return StarSonicResult::SCR_Success;
}

void StarSonic::Task(StarCar* car)
{
    #if _DEBUG
    Serial.println("--> StarSonic::Task()");
    #endif

    if (this->signalPin != PIN_DISCONNECTED
        && car->IsRequested(StarCarSensorRequest::CSR_Sonic)) {
        //Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
        pinMode(this->signalPin, OUTPUT);

        digitalWrite(this->signalPin, LOW);
        delayMicroseconds(2);

        digitalWrite(this->signalPin, HIGH);
        delayMicroseconds(5);

        digitalWrite(this->signalPin, LOW);

        //Same pin is ued to read and receive
        pinMode(this->signalPin, INPUT);

        long duration = pulseIn(this->signalPin, HIGH);
        long distance = duration / 29 / 2; // seconds to cm's

        if (this->location == StarSonicLocation::SCL_Back)
            car->setDistanceBack(distance);
        else if (this->location == StarSonicLocation::SCL_Front)
            car->setDistanceFront(distance);

        #if !TEST
        Serial.print("Sonic: Distance = ");
        Serial.println(distance);
        #endif
    }
}
