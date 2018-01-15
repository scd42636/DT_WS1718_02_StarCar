//--------------------------------------------------------------------------------------------------
// <copyright file="StarBoard.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#pragma once
#include "../StarCar.h"
#include "../../drivers/IbcDriver.h"


enum StarBoardResult
{
    BoardResult_Success = 0,
    BoardResult_Failed = -1,
};

enum StarBoardRequest
{
    BoardRequest_None = 0,
    BoardRequest_ChangeModeToController = 1,
    BoardRequest_ChangeModeToWatch = 2,
    BoardRequest_GetData = 3
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

#if SERIAL_MODE == SERIAL_MODE_ARDUINO
typedef struct StarBoardData_t
{
    byte_t Mode = 0;
    byte_t Request = 0;

    int_t DistanceFront = 0;
    int_t DistanceBack = 0;

    byte_t OrientationParity = 0;
    int_t OrientationValue = 0;

    byte_t AccelerationXParity = 0;
    int_t AccelerationXValue = 0;

    byte_t AccelerationYParity = 0;
    int_t AccelerationYValue = 0;

    int_t Checksum = 0;
} StarBoardData;
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

    bool isStartupSequence;
    StarCarEngineMode previousEngineMode;

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
    void Blink(StarBoardLedPanels panels, int_t times, int_t interval);

    void SignalBlockings(StarCar* car);
    void SignalEngineTurnedOn(StarCar* car);
    void SignalMovement(StarCar* car);

    void SwitchLeds(StarBoardLedPanels panels, bool target);
    virtual void Task(StarCar* car);

    // ---------- Protected methods ----------
protected:
    virtual byte_t InitCore();

    // ---------- Private methods ----------
private:
    void SwitchLed(bool* current, bool target, pin_t pin);

    #if SERIAL_MODE == SERIAL_MODE_ARDUINO
    void ProcessSerialRequests(StarCar* car);
    #endif
};
