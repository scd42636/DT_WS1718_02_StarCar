//--------------------------------------------------------------------------------------------------
// <copyright file="StarBoard.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "StarBoard.h"


// ---------- Public constructors ----------

StarBoard::StarBoard()
{
}

// ---------- Public methods ----------

StarBoardResult StarBoard::Init()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    return StarBoardResult::BR_Success;
}

void StarBoard::Task()
{
}
