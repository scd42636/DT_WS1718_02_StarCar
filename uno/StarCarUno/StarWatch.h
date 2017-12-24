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


enum StarWatchState
{
    WatchState_AccessPoint_Off = 0,
    WatchState_AccessPoint_Idle = 1,
    WatchState_AccessPoint_Listening = 2,
};

enum StarWatchResult
{
    WatchResult_Success = 0,
    WatchResult_Failed = -1,
};

class StarWatch
{
    // ---------- Private fields ----------
private:
    UsbController* controller;
    StarWatchState state;
    bool isFirstRun;

    // ---------- Public constructors ----------
public:
    StarWatch(UsbController* controller);

    // ---------- Public properties ----------
public:
    StarWatchState getState();

    // ---------- Public methods ----------
public:
    StarWatchResult Init();
    void Task(StarCar* car);

    // ---------- Private methods ----------
private:
    bool ReceiveFrame(uint8_t* frame, uint16_t* length);
    bool TransmitFrame(uint8_t* frame, uint16_t length);
};
