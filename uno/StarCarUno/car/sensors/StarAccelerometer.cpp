//--------------------------------------------------------------------------------------------------
// <copyright file="StarAccelerometer.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Annkathrin Bauer</author>
// <author>Simone Huber</author>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "StarAccelerometer.h"


// ---------- Public constructors ----------

StarAccelerometer::StarAccelerometer()
{
    this->isConnected = false;
    this->lastRequestTime = 0;
}

// ---------- Public properties ----------

const char* StarAccelerometer::getName()
{
    return "Accelerometer";
}

// ---------- Public methods ----------

void StarAccelerometer::Task(StarCar* car)
{
    #if _DEBUG
    Serial.println("--> StarAccelerometer::Task()");
    #endif

    ulong_t currentRequestTime = millis();

    // Delay the next request by 100ms to allow the loop to run at 10Hz
    // (default bandwidth for the HMC5883L).
    ulong_t duration = currentRequestTime - this->lastRequestTime;

    if (this->isConnected
        && (this->lastRequestTime == 0 || duration >= 100)
        && car->IsRequested(StarCarSensorRequest::CarSensorRequest_Accelerator)) {
        // Get a new sensor event
        sensors_event_t event;
        this->accelerator.getEvent(&event);

        int_t xAcceleration = event.acceleration.x * 100;
        int_t yAcceleration = event.acceleration.y * 100;

        car->setAccelerationX(xAcceleration);
        car->setAccelerationY(yAcceleration);

        #if TEST
        Serial.print(this->getName());
        Serial.print(": ");

        Serial.print("X=");
        Serial.print(xAcceleration);
        Serial.print(", ");
        Serial.print("Y=");
        Serial.println(yAcceleration);
        #endif

        this->lastRequestTime = currentRequestTime;
    }
}

// ---------- Protected methods ----------

byte_t StarAccelerometer::InitCore()
{
    if (this->accelerator.begin()) {
        this->accelerator.setRange(ADXL345_RANGE_2_G);
        this->isConnected = true;

        return StarAccelerometerResult::AccelerometerResult_NotConnected;
    }

    return StarAccelerometerResult::AccelerometerResult_Success;
}
