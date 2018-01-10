//--------------------------------------------------------------------------------------------------
// <copyright file="StarBoard.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#pragma once
#include "../StarCar.h"
#include "../../drivers/IbcDriver.h"


enum StarBoardResult
{
    BoardResult_Success = 0,
    BoardResult_Failed = -1,
};

class StarBoard : public StarCarModule
{
    // ---------- Private fields ----------
private:
    IbcDriver* ibcDriver;

    bool backLedIsOn;
    pin_t backLedPin;

    bool frontLedIsOn;
    pin_t frontLedPin;

    bool leftLedOn;
    pin_t leftLedPin;

    bool rightLedOn;
    pin_t rightLedPin;

    StarCarEngineMode previousEngineMode;

    // ---------- Public constructors ----------
public:
    StarBoard(
        IbcDriver* ibcDriver,
        pin_t forwardLedPin = PIN_DISCONNECTED,
        pin_t backwardLedPin = PIN_DISCONNECTED,
        pin_t leftFlashLedPin = PIN_DISCONNECTED,
        pin_t rightFlashLedPin = PIN_DISCONNECTED);

    // ---------- Public properties ----------
public:
    virtual const char* getName();

    // ---------- Public methods ----------
public:
    virtual void Task(StarCar* car);

    // ---------- Protected methods ----------
protected:
    virtual byte_t InitCore();

    // ---------- Private methods ----------
private:
    void Blink(short_t times, short_t interval);
    void SwitchLed(bool* current, bool target, pin_t pin);
};
