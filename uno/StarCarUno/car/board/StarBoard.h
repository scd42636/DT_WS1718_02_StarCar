//--------------------------------------------------------------------------------------------------
// <copyright file="StarBoard.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#pragma once
#include "../StarCar.h"
#include "../../drivers/IbcDriver.h"

#if SERIAL_MODE == SERIAL_MODE_LIBRARY
#include <ArduinoSerialProtocol.h>
#endif


enum StarBoardResult
{
    BoardResult_Success = 0,
    BoardResult_Failed = -1,
};

enum StarBoardLedPanels
{
    BoardLedPanel_Front = 1,
    BoardLedPanel_Back = 2,
    BoardLedPanel_Left = 4,
    BoardLedPanel_Right = 8,
    BoardLedPanel_All
    = BoardLedPanel_Front
    | BoardLedPanel_Back
    | BoardLedPanel_Left
    | BoardLedPanel_Right
};

#if SERIAL_MODE == SERIAL_MODE_LIBRARY
typedef struct StarBoardExchangeData_t
{
    StarCarMode Mode;
    StarCarSensorRequest Request;

    int_t DistanceFront;
    int_t DistanceBack;

    byte_t DirectionParity;
    int_t DirectionValue;

    byte_t AccelerationXParity;
    int_t AccelerationXValue;

    byte_t AccelerationYParity;
    int_t AccelerationYValue;
} StarBoardExchangeData;
#endif

class StarBoard : public StarCarModule
{
    // ---------- Private fields ----------
private:
    IbcDriver* ibcDriver;

    bool backLedIsOn;
    pin_t backLedPin;

    bool frontLedIsOn;
    pin_t frontLedPin;

    bool leftLedOn;
    pin_t leftLedPin;

    bool rightLedOn;
    pin_t rightLedPin;

    StarCarEngineMode previousEngineMode;

    #if SERIAL_MODE == SERIAL_MODE_LIBRARY
    StarBoardExchangeData payload;
    ArduinoSerialProtocol* protocol;
    #endif

    // ---------- Public constructors ----------
public:
    StarBoard(
        IbcDriver* ibcDriver,
        pin_t forwardLedPin = PIN_DISCONNECTED,
        pin_t backwardLedPin = PIN_DISCONNECTED,
        pin_t leftFlashLedPin = PIN_DISCONNECTED,
        pin_t rightFlashLedPin = PIN_DISCONNECTED);

    // ---------- Public properties ----------
public:
    virtual const char* getName();

    // ---------- Public methods ----------
public:
    virtual void Task(StarCar* car);

    // ---------- Protected methods ----------
protected:
    virtual byte_t InitCore();

    // ---------- Private methods ----------
private:
    void Blink(StarBoardLedPanels panels, short_t times, short_t interval);
    void SwitchLed(bool* current, bool target, pin_t pin);
};
