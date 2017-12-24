//--------------------------------------------------------------------------------------------------
// <copyright file="StarSonic.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Simone Huber</author>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#pragma once
#include "StarCar.h"


enum StarSonicResult
{
    SonicResult_Success = 0,
    SonicResult_Failed = -1,
};

enum StarSonicLocation
{
    SonicLocation_Front = 0,
    SonicLocation_Back = 1
};

class StarSonic
{
    // ---------- Private fields ----------
private:
    StarSonicLocation location;
    Pin signalPin;

    // ---------- Public constructors ----------
public:
    StarSonic(Pin signalPin, StarSonicLocation location);

    // ---------- Public properties ----------
public:
    StarSonicLocation getLocation();

    // ---------- Public methods ----------
public:
    StarSonicResult Init();
    void Task(StarCar* car);
};
