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
    bool backwardLedIsOn;
    short backwardLedPin;

    bool forwardLedIsOn;
    short forwardLedPin;

    bool leftFlashLedOn;
    short leftFlashLedPin;

    bool rightFlashLedOn;
    short rightFlashLedPin;

    StarCarEngineMode previousEngineMode;

    // ---------- Public constructors ----------
public:
    StarBoard(
        short forwardLedPin = PIN_DISCONNECTED,
        short backwardLedPin = PIN_DISCONNECTED,
        short leftFlashLedPin = PIN_DISCONNECTED,
        short rightFlashLedPin = PIN_DISCONNECTED);

    // ---------- Public methods ----------
public:
    StarBoardResult Init();
    void Task(StarCar* car);

    // ---------- Private methods ----------
private:
    void SwitchLed(bool* current, bool target, short pin);
};
