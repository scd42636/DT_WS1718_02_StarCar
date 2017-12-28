//--------------------------------------------------------------------------------------------------
// <copyright file="StarMagnetometer.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Annkathrin Bauer</author>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#pragma once
#include "../StarCar.h"


enum StarMagnetometerResult
{
    MagnetometerResult_Success = 0,
    MagnetometerResult_Failed = -1,
    MagnetometerResult_NotConnected = -2
};

class StarMagnetometer
{
    // ---------- Private fields ----------
private:
    bool isConnected;
    ulong_t lastRequestTime;
    float_t scaling;

    // ---------- Public constructors ----------
public:
    StarMagnetometer();

    // ---------- Public methods ----------
public:
    StarMagnetometerResult Init();
    void Task(StarCar* car);

    // ---------- Private methods ----------
private:
    void Write(int_t address, int_t data);
};
