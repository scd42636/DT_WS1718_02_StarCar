//--------------------------------------------------------------------------------------------------
// <copyright file="StarWatch.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

// Implemented using the protocol documented on:
// - http://www.instructables.com/id/Control-an-Arduino-With-a-Wristwatch-TI-eZ430-Chr/

#pragma once
#include "StarCar.h"
#include ".\driver\EZ430AccessPoint.h"


enum StarWatchResult
{
    WR_Success = 0,

    WR_Failed = -1,
};

class StarWatch
{
    // ---------- Private fields ----------
private:
    EZ430AccessPoint* accessPoint;

    // ---------- Public constructors ----------
public:
    StarWatch(UsbController* controller);

    // ---------- Public destructor ----------
public:
    ~StarWatch();

    // ---------- Public methods ----------
public:
    StarWatchResult Init();
    void Task();
};
