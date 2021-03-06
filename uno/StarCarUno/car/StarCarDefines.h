//--------------------------------------------------------------------------------------------------
// <copyright file="StarCarDefines.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#pragma once
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

#define HasFlag(flags, bit) ((flags & bit) == bit)

typedef signed char sbyte_t;
typedef unsigned char byte_t;

typedef unsigned short word_t;
typedef unsigned int dword_t;

typedef signed int int_t;
typedef unsigned int uint_t;

typedef signed long long_t;
typedef unsigned long ulong_t;

typedef float float_t;
typedef double double_t;

typedef uint8_t pin_t;

#include <cdcacm.h>
#include <XBOXUSB.h>

#include "../drivers/UsbDriver.h"
typedef XBOXUSB XBoxController;

#define TEST 0
#define _DEBUG 0

#define ENABLE_UHS__DEBUGGING _DEBUG
#define PIN_DISCONNECTED -1


#define SERIAL_MODE_ARDUINO 0
#define SERIAL_MODE_IBC 1

#define SERIAL_MODE SERIAL_MODE_ARDUINO


#define EEPROM_SONIC_FRONT_VALUE        0
#define EEPROM_SONIC_BACK_VALUE         1

#define EEPROM_MAGNETOMETER_PARITY      3
#define EEPROM_MAGNETOMETER_VALUE       4

#define EEPROM_ACCELEROMETER_X_PARITY   5
#define EEPROM_ACCELEROMETER_X_VALUE    6

#define EEPROM_ACCELEROMETER_Y_PARITY   7
#define EEPROM_ACCELEROMETER_Y_VALUE    8


// Implemented using the code provided on:
// - https://github.com/laurb9/StepperDriver/blob/master/src/BasicStepperDriver.h
////static inline void delayMicros(unsigned long delay_us, unsigned long start_us = 0)
////{
////    if (delay_us) {
////        if (!start_us) {
////            start_us = micros();
////        }
////
////        if (delay_us > 50 /* MIN_YIELD_MICROS */) {
////            yield();
////        }
////
////        // See https://www.gammon.com.au/millis
////        while (micros() - start_us < delay_us);
////    }
////}
