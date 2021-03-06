//--------------------------------------------------------------------------------------------------
// <copyright file="StarBoard.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "StarBoard.h"


// ---------- Public constructors ----------

StarBoard::StarBoard(
    IbcDriver* ibcDriver,
    pin_t frontLedPin,
    pin_t backLedPin,
    pin_t leftLedPin,
    pin_t rightLedPin)
{
    this->ibcDriver = ibcDriver;

    this->frontLedPin = frontLedPin;
    this->frontLedIsOn = false;

    this->backLedPin = backLedPin;
    this->backLedIsOn = false;

    this->leftLedPin = leftLedPin;
    this->leftLedOn = false;

    this->rightLedPin = rightLedPin;
    this->rightLedOn = false;

    this->isStartupSequence = false;
    this->previousEngineMode = StarCarEngineMode::CarEngineMode_Off;
}

// ---------- Public properties ----------

const char* StarBoard::getName()
{
    return "Board";
}

// ---------- Public methods ----------

void StarBoard::SwitchLeds(StarBoardLedPanels panels, bool target)
{
    if (HasFlag(panels, StarBoardLedPanels::BoardLedPanel_Front))
        this->SwitchLed(&this->frontLedIsOn, target, this->frontLedPin);

    if (HasFlag(panels, StarBoardLedPanels::BoardLedPanel_Back))
        this->SwitchLed(&this->backLedIsOn, target, this->backLedPin);

    if (HasFlag(panels, StarBoardLedPanels::BoardLedPanel_Right))
        this->SwitchLed(&this->rightLedOn, target, this->rightLedPin);

    if (HasFlag(panels, StarBoardLedPanels::BoardLedPanel_Left))
        this->SwitchLed(&this->leftLedOn, target, this->leftLedPin);
}

void StarBoard::Task(StarCar* car)
{
    if (this->isStartupSequence) {
        this->isStartupSequence = false;
        this->SwitchLeds(StarBoardLedPanels::BoardLedPanel_All, false);
    }

    #if _DEBUG
    Serial.println("--> StarBoard::Task()");
    #endif

    #if TEST
    int value = millis() / 10;

    car->setAccelerationX(value)
        ->setAccelerationY(value)
        ->setDistanceBack(value)
        ->setDistanceFront(value)
        ->setOrientation(value);
    #endif

    #if SERIAL_MODE == SERIAL_MODE_ARDUINO
    this->ProcessSerialRequests(car);
    #elif SERIAL_MODE == SERIAL_MODE_IBC
    this->ibcDriver->next(car);
    #endif

    StarCarEngineMode currentEngineMode = car->getEngineMode();

    if (this->previousEngineMode != currentEngineMode
        && currentEngineMode == StarCarEngineMode::CarEngineMode_On) {
        this->SignalEngineTurnedOn(car);
    }

    if (currentEngineMode == StarCarEngineMode::CarEngineMode_On) {
        if (car->IsBlocked())
            this->SignalBlockings(car);
        else
            this->SignalMovement(car);
    }

    this->previousEngineMode = currentEngineMode;
}

// ---------- Protected methods ----------

byte_t StarBoard::InitCore()
{
    this->isStartupSequence = true;

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    if (this->frontLedPin != PIN_DISCONNECTED) {
        pinMode(this->frontLedPin, OUTPUT);
        digitalWrite(this->frontLedPin, LOW);
    }

    if (this->backLedPin != PIN_DISCONNECTED) {
        pinMode(this->backLedPin, OUTPUT);
        digitalWrite(this->backLedPin, LOW);
    }

    if (this->leftLedPin != PIN_DISCONNECTED) {
        pinMode(this->leftLedPin, OUTPUT);
        digitalWrite(this->leftLedPin, LOW);
    }

    if (this->rightLedPin != PIN_DISCONNECTED) {
        pinMode(this->rightLedPin, OUTPUT);
        digitalWrite(this->rightLedPin, LOW);
    }

    this->SwitchLeds(StarBoardLedPanels::BoardLedPanel_All, true);
    return StarBoardResult::BoardResult_Success;
}

// ---------- Private methods ----------

void StarBoard::Blink(StarBoardLedPanels panels, int_t times, int_t interval)
{
    for (int_t index = 0; index < times; index++) {
        this->SwitchLeds(panels, true);
        delay(interval);

        this->SwitchLeds(panels, false);
        delay(interval);
    }
}

void StarBoard::SignalBlockings(StarCar* car)
{
    StarBoardLedPanels panels = StarBoardLedPanels::BoardLedPanel_All;

    if (car->IsFrontBlocked(/*exclusive*/ true))
        panels = StarBoardLedPanels::BoardLedPanel_Front;
    else if (car->IsBackBlocked(/*exclusive*/ true))
        panels = StarBoardLedPanels::BoardLedPanel_Back;

    this->Blink(panels, /*times:*/ 3, /*interval:*/ 50);
}

void StarBoard::SignalEngineTurnedOn(StarCar* car)
{
    this->Blink(
        StarBoardLedPanels::BoardLedPanel_All,
        /*times:*/ 5,
        /*interval:*/ 100);
}

void StarBoard::SignalMovement(StarCar* car)
{
    sbyte_t speed = car->getSpeed();

    this->SwitchLed(&this->frontLedIsOn, speed > 10, this->frontLedPin);
    this->SwitchLed(&this->backLedIsOn, speed < -10, this->backLedPin);

    sbyte_t direction = car->getDirection();

    this->SwitchLed(&this->rightLedOn, direction > 0, this->rightLedPin);
    this->SwitchLed(&this->leftLedOn, direction < 0, this->leftLedPin);
}

void StarBoard::SwitchLed(bool* current, bool target, pin_t pin)
{
    if (pin != PIN_DISCONNECTED) {
        if (*current != target) {
            if (target)
                digitalWrite(pin, HIGH);
            else
                digitalWrite(pin, LOW);

            *current = target;
        }
    }
}

#if SERIAL_MODE == SERIAL_MODE_ARDUINO
void StarBoard::ProcessSerialRequests(StarCar* car)
{
    if (Serial.available()) {
        int_t value = Serial.read();

        if (value >= 48 && value <= 57)
            value = value - 48;

        StarBoardRequest request = (StarBoardRequest)value;

        if (request > StarBoardRequest::BoardRequest_None) {
            if (request == StarBoardRequest::BoardRequest_GetData) {
                StarBoardData data;

                data.Mode = car->getMode();
                data.Checksum ^= data.Mode;

                data.Request = car->getRequest();
                data.Checksum ^= data.Request;

                data.DistanceFront = car->getDistanceFront();
                data.Checksum ^= data.DistanceFront;

                data.DistanceBack = car->getDistanceBack();
                data.Checksum ^= data.DistanceBack;

                data.OrientationValue = car->getOrientation();

                if (data.OrientationValue < 0) {
                    data.OrientationParity = 1;
                    data.OrientationValue = -data.OrientationValue;
                }

                data.Checksum ^= data.OrientationParity;
                data.Checksum ^= data.OrientationValue;

                data.AccelerationXValue = car->getAccelerationX();

                if (data.AccelerationXValue < 0) {
                    data.AccelerationXParity = 1;
                    data.AccelerationXValue = -data.AccelerationXValue;
                }

                data.Checksum ^= data.AccelerationXParity;
                data.Checksum ^= data.AccelerationXValue;

                data.AccelerationYValue = car->getAccelerationY();

                if (data.AccelerationYValue < 0) {
                    data.AccelerationYParity = 1;
                    data.AccelerationYValue = -data.AccelerationYValue;
                }

                data.Checksum ^= data.AccelerationYParity;
                data.Checksum ^= data.AccelerationYValue;

                Serial.write((const uint8_t*)&data, sizeof(StarBoardData));
            }
            else {
                car->setMode((StarCarMode)value);
            }
        }
    }
}
#endif
