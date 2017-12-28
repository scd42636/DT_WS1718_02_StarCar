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
    this->accelerationX = 0;
    this->accelerationY = 0;
    this->direction = 0;
    this->distanceBack = 0;
    this->distanceFront = 0;
    this->engineMode = StarCarEngineMode::CarEngineMode_Off;
    this->mode = StarCarMode::CarMode_None;
    this->orientation = 0;
    this->request = StarCarSensorRequest::CarSensorRequest_None;
    this->speed = 0;
}

// ---------- Public properties ----------

int_t StarCar::getAccelerationX()
{
    return this->accelerationX;
}
StarCar* StarCar::setAccelerationX(int_t value)
{
    this->accelerationX = value;
    return this;
}

int_t StarCar::getAccelerationY()
{
    return this->accelerationY;
}
StarCar* StarCar::setAccelerationY(int_t value)
{
    this->accelerationY = value;
    return this;
}

sbyte_t StarCar::getDirection()
{
    return this->direction;
}
StarCar* StarCar::setDirection(sbyte_t value)
{
    this->direction = value;
    return this;
}

short_t StarCar::getDistanceBack()
{
    return this->distanceBack;
}
StarCar* StarCar::setDistanceBack(short_t value)
{
    this->distanceBack = value;
    return this;
}

short_t StarCar::getDistanceFront()
{
    return this->distanceFront;
}
StarCar* StarCar::setDistanceFront(short_t value)
{
    this->distanceFront = value;
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

StarCarEngineMode StarCar::getEngineMode()
{
    return this->engineMode;
}
StarCar* StarCar::setEngineMode(StarCarEngineMode value)
{
    this->engineMode = value;
    return this;
}

short_t StarCar::getOrientation()
{
    return this->orientation;
}
StarCar* StarCar::setOrientation(short_t value)
{
    this->orientation = value;
    return this;
}

StarCarSensorRequest StarCar::getRequest()
{
    return this->request;
}
StarCar* StarCar::setRequest(StarCarSensorRequest value)
{
    this->request = value;
    return this;
}

sbyte_t StarCar::getSpeed()
{
    return this->speed;
}
StarCar* StarCar::setSpeed(sbyte_t value)
{
    this->speed = value;
    return this;
}

// ---------- Public methods ----------

bool StarCar::IsRequested(StarCarSensorRequest request)
{
    return (this->request & request) == request;
}
