//--------------------------------------------------------------------------------------------------
// <copyright file="StarCar.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#pragma once
#include "StarCarDefines.h"


enum StarCarMode
{
    CarMode_None = 0,
    CarMode_Controller = 1,
    CarMode_Watch = 2
};

enum StarCarEngineMode
{
    CarEngineMode_Off = 0,
    CarEngineMode_On = 1
};

enum StarCarSensorRequest
{
    CarSensorRequest_None = 0,
    CarSensorRequest_Sonic = 1,
    CarSensorRequest_Magnet = 2,
    CarSensorRequest_Accelerator = 4
};

class StarCar
{
    // ---------- Private fields ----------
private:
    int_t accelerationX;
    int_t accelerationY;
    sbyte_t direction;
    short_t distanceBack;
    short_t distanceFront;
    StarCarEngineMode engineMode;
    StarCarMode mode;
    short_t orientation;
    StarCarSensorRequest request;
    sbyte_t speed;

    // ---------- Public constructors ----------
public:
    StarCar();

    // ---------- Public properties ----------
public:
    int_t getAccelerationX();
    StarCar* setAccelerationX(int_t value);

    int_t getAccelerationY();
    StarCar* setAccelerationY(int_t value);

    sbyte_t getDirection();
    StarCar* setDirection(sbyte_t value);

    short_t getDistanceBack();
    StarCar* setDistanceBack(short_t value);

    short_t getDistanceFront();
    StarCar* setDistanceFront(short_t value);

    StarCarMode getMode();
    StarCar* setMode(StarCarMode value);

    short_t getOrientation();
    StarCar* setOrientation(short_t value);

    StarCarEngineMode getEngineMode();
    StarCar* setEngineMode(StarCarEngineMode value);

    StarCarSensorRequest getRequest();
    StarCar* setRequest(StarCarSensorRequest value);

    sbyte_t getSpeed();
    StarCar* setSpeed(sbyte_t value);

    // ---------- Public methods ----------
public:
    bool IsRequested(StarCarSensorRequest request);
};
