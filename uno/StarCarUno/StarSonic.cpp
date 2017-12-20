//--------------------------------------------------------------------------------------------------
// <copyright file="StarSonic.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "StarSonic.h"


// ---------- Public constructors ----------

StarSonic::StarSonic()
{
}

// ---------- Public methods ----------

StarSonicResult StarSonic::Init()
{
    return StarSonicResult::SCR_Success;
}

void StarSonic::Task(StarCar* car)
{
    #if _DEBUG
    Serial.println("--> StarSonic::Task()");
    #endif
}
