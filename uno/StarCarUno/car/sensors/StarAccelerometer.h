//--------------------------------------------------------------------------------------------------
// <copyright file="StarAccelerometer.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Annkathrin Bauer</author>
// <author>Simone Huber</author>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#pragma once
#include "../StarCar.h"

#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>


enum StarAccelerometerResult
{
    AccelerometerResult_Success = 0,
    AccelerometerResult_Failed = -1,
    AccelerometerResult_NotConnected = -2
};

class StarAccelerometer
{
    // ---------- Private fields ----------
private:
    Adafruit_ADXL345_Unified accelerator = Adafruit_ADXL345_Unified(12345);
    bool isConnected;
    ulong_t lastRequestTime;

    // ---------- Public constructors ----------
public:
    StarAccelerometer();

    // ---------- Public methods ----------
public:
    StarAccelerometerResult Init();
    void Task(StarCar* car);
};
