//--------------------------------------------------------------------------------------------------
// <copyright file="StarController.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

// Implemented using the protocol documented on:
// - https://github.com/felis/USB_Host_Shield_2.0/tree/master/examples/Xbox/XBOXUSB/

#pragma once
#include "../StarCar.h"


enum StarControllerResult
{
    ControllerResult_Success = 0,
    ControllerResult_Failed = -1,
};

enum StarControllerMode
{
    Hat = 0,
    Button = 1
};

class StarController : public StarCarModule
{
    // ---------- Private fields ----------
private:
    StarControllerMode mode;
    bool modeIsActive;
    XBoxController* xboxController;

    // ---------- Public constructors ----------
public:
    StarController(XBoxController* xboxController);

    // ---------- Public properties ----------
public:
    StarControllerMode getMode();
    virtual const char* getName();

    // ---------- Public methods ----------
public:
    virtual void Task(StarCar* car);

    // ---------- Protected methods ----------
protected:
    virtual byte_t InitCore();
};
