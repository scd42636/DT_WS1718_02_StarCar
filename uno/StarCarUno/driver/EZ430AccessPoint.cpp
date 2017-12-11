//--------------------------------------------------------------------------------------------------
// <copyright file="EZ430AccessPoint.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "EZ430AccessPoint.h"


// ---------- Public constructors ----------

EZ430AccessPoint::EZ430AccessPoint(UsbController* controller)
{
    this->controller = controller;
}

// ---------- Public methods ----------
