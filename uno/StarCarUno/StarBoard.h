//--------------------------------------------------------------------------------------------------
// <copyright file="StarBoard.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#pragma once
#include "StarCar.h"
#include <cdcacm.h>


enum StarBoardResult
{
    BR_Success = 0,
    BR_Failed = -1,
};

class StarBoard
{
    // ---------- Public constructors ----------
public:
    StarBoard();

    // ---------- Public methods ----------
public:
    StarBoardResult Init();
    void Task();
};
