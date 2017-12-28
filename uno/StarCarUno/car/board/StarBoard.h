//--------------------------------------------------------------------------------------------------
// <copyright file="StarBoard.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#pragma once
#include "../StarCar.h"


enum StarBoardResult
{
    BoardResult_Success = 0,
    BoardResult_Failed = -1,
};

class StarBoard
{
    // ---------- Private fields ----------
public:
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
        pin_t forwardLedPin = PIN_DISCONNECTED,
        pin_t backwardLedPin = PIN_DISCONNECTED,
        pin_t leftFlashLedPin = PIN_DISCONNECTED,
        pin_t rightFlashLedPin = PIN_DISCONNECTED);

    // ---------- Public methods ----------
public:
    StarBoardResult Init();
    void Task(StarCar* car);

    // ---------- Private methods ----------
private:
    void SwitchLed(bool* current, bool target, pin_t pin);
};
