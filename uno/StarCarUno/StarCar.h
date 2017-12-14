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

class StarCar
{
    // ---------- Private fields ----------
private:
    int8_t direction;
    int8_t speed;
    StarCarMode mode;

    // ---------- Public constructors ----------
public:
    StarCar();

    // ---------- Public properties ----------
public:
    int8_t getDirection();
    StarCar* setDirection(int8_t value);

    StarCarMode getMode();
    StarCar* setMode(StarCarMode mode);

    int8_t getSpeed();
    StarCar* setSpeed(int8_t value);
};
