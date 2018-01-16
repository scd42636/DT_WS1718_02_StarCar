//--------------------------------------------------------------------------------------------------
// <copyright file="StarCar.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "StarCar.h"


// ---------- Public constructors ----------

StarCar::StarCar(StarCarModule** modules, int_t modulesLength)
{
    this->accelerationX = 0;
    this->accelerationY = 0;
    this->blockings = StarCarBlockings::CarBlocking_None;
    this->direction = 0;
    this->distanceBack = 0;
    this->distanceFront = 0;
    this->engineMode = StarCarEngineMode::CarEngineMode_Off;
    this->mode = StarCarMode::CarMode_None;
    this->orientation = 0.0;
    this->request = StarCarSensorRequest::CarSensorRequest_All;
    this->speed = 0;

    this->modulesLength = modulesLength;
    this->modules = modules;
}

// ---------- Public properties ----------

int_t StarCar::getAccelerationX()
{
    return this->accelerationX;
}
StarCar* StarCar::setAccelerationX(int_t value)
{
    this->accelerationX = value;

    if (value < 0) {
        EEPROM.write(EEPROM_ACCELEROMETER_X_PARITY, 1);
        EEPROM.write(EEPROM_ACCELEROMETER_X_VALUE, -value);
    }
    else {
        EEPROM.write(EEPROM_ACCELEROMETER_X_PARITY, 0);
        EEPROM.write(EEPROM_ACCELEROMETER_X_VALUE, value);
    }

    return this;
}

int_t StarCar::getAccelerationY()
{
    return this->accelerationY;
}
StarCar* StarCar::setAccelerationY(int_t value)
{
    this->accelerationY = value;

    if (value < 0) {
        EEPROM.write(EEPROM_ACCELEROMETER_Y_PARITY, 1);
        EEPROM.write(EEPROM_ACCELEROMETER_Y_VALUE, -value);
    }
    else {
        EEPROM.write(EEPROM_ACCELEROMETER_Y_PARITY, 0);
        EEPROM.write(EEPROM_ACCELEROMETER_Y_VALUE, value);
    }

    return this;
}

sbyte_t StarCar::getDirection()
{
    return this->direction;
}
StarCar* StarCar::setDirection(sbyte_t value)
{
    if (value >= -100 && value <= 100)
        this->direction = value;

    return this;
}

int_t StarCar::getDistanceBack()
{
    return this->distanceBack;
}
StarCar* StarCar::setDistanceBack(int_t value)
{
    if (value < this->CalculateCrashPreventionDistance()) {
        if (this->speed < 0)
            this->speed = 0;

        this->blockings = (StarCarBlockings)
            (this->blockings
            | StarCarBlockings::CarBlocking_Back);
    }
    else {
        this->blockings = (StarCarBlockings)
            (this->blockings
            & ~StarCarBlockings::CarBlocking_Back);
    }

    this->distanceBack = value;
    EEPROM.write(EEPROM_SONIC_BACK_VALUE, value);

    return this;
}

int_t StarCar::getDistanceFront()
{
    return this->distanceFront;
}
StarCar* StarCar::setDistanceFront(int_t value)
{
    if (value < this->CalculateCrashPreventionDistance()) {
        if (this->speed > 0)
            this->speed = 0;

        this->blockings = (StarCarBlockings)
            (this->blockings
            | StarCarBlockings::CarBlocking_Front);
    }
    else {
        this->blockings = (StarCarBlockings)
            (this->blockings
            & ~StarCarBlockings::CarBlocking_Front);
    }

    this->distanceFront = value;
    EEPROM.write(EEPROM_SONIC_FRONT_VALUE, value);

    return this;
}

StarCarMode StarCar::getMode()
{
    return this->mode;
}
StarCar* StarCar::setMode(StarCarMode value)
{
    this->mode = value;
    this->engineMode = StarCarEngineMode::CarEngineMode_Off;

    #if _DEBUG
    Serial.print("Mode Change = ");
    Serial.println(this->mode);
    #endif

    return this;
}

StarCarEngineMode StarCar::getEngineMode()
{
    return this->engineMode;
}
StarCar* StarCar::setEngineMode(StarCarEngineMode value)
{
    this->engineMode = value;
    this->speed = 0;

    return this;
}

float_t StarCar::getOrientation()
{
    return this->orientation;
}
StarCar* StarCar::setOrientation(float_t value)
{
    this->orientation = value;

    if (value < 0) {
        EEPROM.write(EEPROM_MAGNETOMETER_PARITY, 1);
        EEPROM.write(EEPROM_MAGNETOMETER_VALUE, -value);
    }
    else {
        EEPROM.write(EEPROM_MAGNETOMETER_PARITY, 0);
        EEPROM.write(EEPROM_MAGNETOMETER_VALUE, value);
    }

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
    if (value >= -100 && value <= 100) {
        if ((value < 0 && this->IsBackBlocked(/*exclusive*/true))
            || (value > 0 && this->IsFrontBlocked(/*exclusive*/true))) {
            this->speed = 0;
        }
        else {
            this->speed = value;
        }
    }

    return this;
}

// ---------- Public methods ----------

sbyte_t StarCar::CalculateCrashPreventionDistance()
{
    sbyte_t distance = 15;
    float_t speedValue = this->speed;

    if (speedValue < 0)
        speedValue *= (-1);

    ////distance += (sbyte_t)((float_t)speedValue * 0.25);
    distance = ((speedValue / 10.0) * (speedValue / 10.0)) / 2.0;

    return distance + 5;
}

bool StarCar::IsBlocked()
{
    return this->blockings != StarCarBlockings::CarBlocking_None;
}

bool StarCar::IsBackBlocked(bool exclusive)
{
    if (exclusive)
        return this->blockings == StarCarBlockings::CarBlocking_Back;

    return (this->blockings & StarCarBlockings::CarBlocking_Back) == StarCarBlockings::CarBlocking_Back;
}

bool StarCar::IsFrontBlocked(bool exclusive)
{
    if (exclusive)
        return this->blockings == StarCarBlockings::CarBlocking_Front;

    return (this->blockings & StarCarBlockings::CarBlocking_Front) == StarCarBlockings::CarBlocking_Front;
}

bool StarCar::IsRequested(StarCarSensorRequest request)
{
    return (this->request & request) == request;
}

////void StarCar::RegisterModule(StarCarModule* module)
////{
////    bool found = false;
////    int_t index = 0;
////
////    for (; index < this->modulesLength; index++) {
////        StarCarModule* registeredModule = this->modules[index];
////
////        if (registeredModule == module) {
////            found = true;
////            break;
////        }
////    }
////
////    if (!found)
////        this->modules[index] = module;
////}
////
////void StarCar::UnregisterModule(StarCarModule* module)
////{
////    int_t index = 0;
////
////    for (; index < this->modulesLength; index++) {
////        StarCarModule* registeredModule = this->modules[index];
////
////        if (registeredModule == module) {
////            this->modules[index] = nullptr;
////            break;
////        }
////    }
////}

void StarCar::Init()
{
    for (int_t index = 0; index < this->modulesLength; index++) {
        StarCarModule* module = this->modules[index];

        if (module != nullptr)
            module->Init();
    }
}

void StarCar::Task()
{
    for (int_t index = 0; index < this->modulesLength; index++) {
        StarCarModule* module = this->modules[index];

        if (module != nullptr)
            module->Task(this);
    }
}

void StarCar::SwitchEngineMode()
{
    StarCarEngineMode engineMode = this->engineMode;

    if (engineMode == StarCarEngineMode::CarEngineMode_Off)
        this->engineMode = StarCarEngineMode::CarEngineMode_On;
    else
        this->engineMode = StarCarEngineMode::CarEngineMode_Off;

    this->speed = 0;
}
