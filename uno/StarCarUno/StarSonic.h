//--------------------------------------------------------------------------------------------------
// <copyright file="StarSonic.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#pragma once
#include "StarCar.h"


enum StarSonicResult
{
    SCR_Success = 0,
    SCR_Failed = -1,
};

class StarSonic
{
    // ---------- Public constructors ----------
public:
    StarSonic();

    // ---------- Public methods ----------
public:
    StarSonicResult Init();
    void Task(StarCar* car);
};
