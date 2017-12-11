//--------------------------------------------------------------------------------------------------
// <copyright file="EZ430AccessPoint.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

// Implemented using the protocol documented on:
// - http://www.instructables.com/id/Control-an-Arduino-With-a-Wristwatch-TI-eZ430-Chr/

#pragma once
#include "../StarCar.h"
#include "UsbDriver.h"


enum EZ430AccessPointResult
{
    AP_Success = 0,

    AP_Failed = -1,
    AP_USB_Shield_InitFailed = -2,
};

class EZ430AccessPoint
{
    // ---------- Private fields ----------
private:
    UsbController* controller;

    // ---------- Public constructors ----------
public:
    EZ430AccessPoint(UsbController* controller);

    // ---------- Public methods ----------
public:
};
