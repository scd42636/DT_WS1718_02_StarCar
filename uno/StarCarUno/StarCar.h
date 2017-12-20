//--------------------------------------------------------------------------------------------------
// <copyright file="StarCar.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#pragma once
#include "StarCarDefines.h"


enum StarCarMode
{
    CM_None = 0,
    CM_Controller = 1,
    CM_Watch = 2
};

enum StarCarEngineMode
{
    CEM_Off = 0,
    CEM_On = 1
};

class StarCar
{
    // ---------- Private fields ----------
private:
    int8_t acceleration;
    int8_t direction;
    StarCarEngineMode engineMode;
    int8_t speed;
    StarCarMode mode;

    // ---------- Public constructors ----------
public:
    StarCar();

    // ---------- Public properties ----------
public:
    int8_t getAcceleration();
    StarCar* setAcceleration(int8_t value);

    int8_t getDirection();
    StarCar* setDirection(int8_t value);

    StarCarMode getMode();
    StarCar* setMode(StarCarMode value);

    StarCarEngineMode getEngineMode();
    StarCar* setEngineMode(StarCarEngineMode value);
};
