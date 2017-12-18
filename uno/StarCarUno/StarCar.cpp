//--------------------------------------------------------------------------------------------------
// <copyright file="StarCar.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "StarCar.h"


// ---------- Public constructors ----------

StarCar::StarCar()
{
    this->direction = 0;
    this->mode = StarCarMode::CM_None;
}

// ---------- Public properties ----------

int8_t StarCar::getDirection()
{
    return this->direction;
}
StarCar* StarCar::setDirection(int8_t value)
{
    this->direction = value;
    return this;
}

StarCarMode StarCar::getMode()
{
    return this->mode;
}
StarCar* StarCar::setMode(StarCarMode value)
{
    this->mode = value;

    Serial.print("Mode Change = ");
    Serial.println(this->mode);

    return this;
}

int8_t StarCar::getSpeed()
{
    return this->speed;
}
StarCar* StarCar::setSpeed(int8_t value)
{
    this->speed = value;
    return this;
}
