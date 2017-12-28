//--------------------------------------------------------------------------------------------------
// <copyright file="UsbDriver.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

// Implemented using the protocol documented on:
// - http://www.instructables.com/id/Control-an-Arduino-With-a-Wristwatch-TI-eZ430-Chr/

#pragma once
#include <cdcacm.h>

typedef USB Usb;
typedef ACM UsbController;
typedef CDCAsyncOper UsbDriverAbstract;

class UsbDriver : public UsbDriverAbstract
{
    // ---------- Public constructors ----------
////public:
////    UsbDriver();

    // ---------- Public methods ----------
public:
    virtual uint8_t OnInit(UsbController* controller);
};
