//--------------------------------------------------------------------------------------------------
// <copyright file="StarWatch.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "StarWatch.h"


// ---------- Public constructors ----------

StarWatch::StarWatch(UsbController* controller)
{
    this->accessPoint = new EZ430AccessPoint(controller);
}

// ---------- Public destructor ----------

StarWatch::~StarWatch()
{
    delete this->accessPoint;
}

// ---------- Public methods ----------

StarWatchResult StarWatch::Init()
{
    return StarWatchResult::WR_Success;
}

void StarWatch::Task()
{
    #if _DEBUG
    Serial.println("--> StarWatch::Task()");
    #endif
}
