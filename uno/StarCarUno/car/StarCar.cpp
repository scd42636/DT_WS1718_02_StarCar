//--------------------------------------------------------------------------------------------------
// <copyright file="StarCar.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "StarCar.h"


// ---------- Public constructors ----------

StarCar::StarCar(StarCarModule** modules, short_t modulesLength)
{
    this->accelerationX = 0;
    this->accelerationY = 0;
    this->blockings = StarCarBlockings::CarBlocking_None;
    this->direction = 0;
    this->distanceBack = 0;
    this->distanceFront = 0;
    this->engineMode = StarCarEngineMode::CarEngineMode_Off;
    this->mode = StarCarMode::CarMode_None;
    this->orientation = 0;
    this->request = StarCarSensorRequest::CarSensorRequest_None;
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
    //EEPROM.put(9, value);

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
    //EEPROM.put(10, value);

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
    this->direction = value;
    return this;
}

short_t StarCar::getDistanceBack()
{
    return this->distanceBack;
}
StarCar* StarCar::setDistanceBack(short_t value)
{
    if (this->speed < 0 && value < 15) {
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

short_t StarCar::getDistanceFront()
{
    return this->distanceFront;
}
StarCar* StarCar::setDistanceFront(short_t value)
{
    if (this->speed > 0 && value < 15) {
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
    if (this->IsBlocked()) {
        this->speed = 0;
    }
    else {
        this->speed = value;
    }

    return this;
}

// ---------- Public methods ----------

bool StarCar::IsBlocked()
{
    return this->blockings != StarCarBlockings::CarBlocking_None;
}

bool StarCar::IsRequested(StarCarSensorRequest request)
{
    return (this->request & request) == request;
}

////void StarCar::RegisterModule(StarCarModule* module)
////{
////    bool found = false;
////    short_t index = 0;
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
////    short_t index = 0;
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
    for (short_t index = 0; index < this->modulesLength; index++) {
        StarCarModule* module = this->modules[index];

        if (module != nullptr)
            module->Init();
    }
}

void StarCar::Task()
{
    for (short_t index = 0; index < this->modulesLength; index++) {
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
