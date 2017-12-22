//--------------------------------------------------------------------------------------------------
// <copyright file="StarBoard.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#pragma once
#include "StarCar.h"


enum StarBoardResult
{
    BR_Success = 0,
    BR_Failed = -1,
};

class StarBoard
{
    // ---------- Private fields ----------
public:
    bool backLedIsOn;
    Pin backLedPin;

    bool frontLedIsOn;
    Pin frontLedPin;

    bool leftLedOn;
    Pin leftLedPin;

    bool rightLedOn;
    Pin rightLedPin;

    StarCarEngineMode previousEngineMode;

    // ---------- Public constructors ----------
public:
    StarBoard(
        Pin forwardLedPin = PIN_DISCONNECTED,
        Pin backwardLedPin = PIN_DISCONNECTED,
        Pin leftFlashLedPin = PIN_DISCONNECTED,
        Pin rightFlashLedPin = PIN_DISCONNECTED);

    // ---------- Public methods ----------
public:
    StarBoardResult Init();
    void Task(StarCar* car);

    // ---------- Private methods ----------
private:
    void SwitchLed(bool* current, bool target, short pin);
};
