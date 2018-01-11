//--------------------------------------------------------------------------------------------------
// <copyright file="IbcDriver.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Robert Graf</author>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#pragma once
#include <Arduino.h>
#include <EEPROM.h>
//#include "../car/StarCar.h"

#define EEPROM_SONIC_FRONT_VALUE        0
#define EEPROM_SONIC_BACK_VALUE         1

#define EEPROM_MAGNETOMETER_PARITY      3
#define EEPROM_MAGNETOMETER_VALUE       4

#define EEPROM_ACCELEROMETER_X_PARITY   5
#define EEPROM_ACCELEROMETER_X_VALUE    6

#define EEPROM_ACCELEROMETER_Y_PARITY   7
#define EEPROM_ACCELEROMETER_Y_VALUE    8


class IbcDriver
{
private:
    byte DH;
    int count = 0;

public:
    IbcDriver(int baud);
    void next(void* car);

    void send(byte* d, int length);
    void send(byte d);
    int recv(byte*d, int length, bool block = false);
    byte recv(bool block = false);

    void setDH(byte dh);

    byte createDH(byte* b, int lenght);
    byte createDH(byte* b, int lenght, byte in);
};
