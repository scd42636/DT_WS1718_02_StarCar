//--------------------------------------------------------------------------------------------------
// <copyright file="StarCarModule.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#pragma once
#include "StarCarDefines.h"

class StarCar;

class StarCarModule
{
    // ---------- Protected constructors ----------
protected:
    StarCarModule();

    // ---------- Public properties ----------
public:
    virtual const char* getName() = 0;

    // ---------- Public methods ----------
public:
    void Init();
    virtual void Task(StarCar* car) = 0;

    // ---------- Protected methods ----------
protected:
    virtual byte_t InitCore() = 0;
};
