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
    this->acceleration = 0;
    this->direction = 0;
    this->distanceBack = 0;
    this->distanceFront = 0;
    this->mode = StarCarMode::CM_None;
    this->engineMode = StarCarEngineMode::CEM_Off;
    this->request = StarCarSensorRequest::CSR_None;
}

// ---------- Public properties ----------

int8_t StarCar::getAcceleration()
{
    return this->acceleration;
}
StarCar* StarCar::setAcceleration(int8_t value)
{
    this->acceleration = value;
    return this;
}

int8_t StarCar::getDirection()
{
    return this->direction;
}
StarCar* StarCar::setDirection(int8_t value)
{
    this->direction = value;
    return this;
}

int8_t StarCar::getDistanceBack()
{
    return this->distanceBack;
}
StarCar* StarCar::setDistanceBack(int8_t value)
{
    this->distanceBack = value;
    return this;
}

int8_t StarCar::getDistanceFront()
{
    return this->distanceFront;
}
StarCar* StarCar::setDistanceFront(int8_t value)
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

StarCarSensorRequest StarCar::getRequest()
{
    return this->request;
}
StarCar* StarCar::setRequest(StarCarSensorRequest value)
{
    this->request = value;
    return this;
}

// ---------- Public methods ----------

bool StarCar::IsRequested(StarCarSensorRequest request)
{
    return (this->request & request) == request;
}
