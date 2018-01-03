//--------------------------------------------------------------------------------------------------
// <copyright file="StarSonic.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Simone Huber</author>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#pragma once
#include "../StarCar.h"


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

class StarSonic : public StarCarModule
{
    // ---------- Private fields ----------
private:
    StarSonicLocation location;
    pin_t signalPin;

    // ---------- Public constructors ----------
public:
    StarSonic(pin_t signalPin, StarSonicLocation location);

    // ---------- Public properties ----------
public:
    StarSonicLocation getLocation();
    virtual const char* getName();

    // ---------- Public methods ----------
public:
    void Task(StarCar* car);

    // ---------- Protected methods ----------
protected:
    virtual byte_t InitCore();
};
