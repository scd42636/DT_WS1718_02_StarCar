//--------------------------------------------------------------------------------------------------
// <copyright file="StarCarModule.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "StarCarModule.h"


// ---------- Protected constructors ----------

StarCarModule::StarCarModule()
{
}

// ---------- Public properties ----------

void StarCarModule::Init()
{
    #if !_DEBUG
    delay(100);
    Serial.print("--> Initializing ");
    Serial.print(this->getName());
    Serial.print("...\t\t\t\t");
    #endif

    short_t status = this->InitCore();

    if (status == 0) {
        #if !_DEBUG
        Serial.println("success!");
        #endif
    }
    else {
        #if !_DEBUG
        Serial.println("FAILED!");
        #endif
    }
}
