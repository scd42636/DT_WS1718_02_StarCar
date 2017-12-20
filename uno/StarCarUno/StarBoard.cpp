//--------------------------------------------------------------------------------------------------
// <copyright file="StarBoard.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "StarBoard.h"


// ---------- Public constructors ----------

StarBoard::StarBoard(
    short forwardLedPin,
    short backwardLedPin,
    short leftFlashLedPin,
    short rightFlashLedPin)
{
    this->forwardLedPin = forwardLedPin;
    this->forwardLedIsOn = false;

    this->backwardLedPin = backwardLedPin;
    this->forwardLedIsOn = false;

    this->leftFlashLedPin = leftFlashLedPin;
    this->leftFlashLedOn = false;

    this->rightFlashLedPin = rightFlashLedPin;
    this->rightFlashLedOn = false;

    this->previousEngineMode = StarCarEngineMode::CEM_Off;
}

// ---------- Public methods ----------

StarBoardResult StarBoard::Init()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    if (this->forwardLedPin != PIN_DISCONNECTED) {
        pinMode(this->forwardLedPin, OUTPUT);
        digitalWrite(this->forwardLedPin, LOW);
    }

    if (this->backwardLedPin != PIN_DISCONNECTED) {
        pinMode(this->backwardLedPin, OUTPUT);
        digitalWrite(this->backwardLedPin, LOW);
    }

    if (this->leftFlashLedPin != PIN_DISCONNECTED) {
        pinMode(this->leftFlashLedPin, OUTPUT);
        digitalWrite(this->leftFlashLedPin, LOW);
    }

    if (this->rightFlashLedPin != PIN_DISCONNECTED) {
        pinMode(this->rightFlashLedPin, OUTPUT);
        digitalWrite(this->rightFlashLedPin, LOW);
    }

    return StarBoardResult::BR_Success;
}

void StarBoard::Task(StarCar* car)
{
    #if _DEBUG
    Serial.println("--> StarBoard::Task()");
    #endif

    if (Serial.available()) {
        int value = Serial.read();

        if (value >= 48 && value <= 57) {
            StarCarMode mode = (StarCarMode)(value - 48);
            car->setMode(mode);
        }
    }

    StarCarEngineMode currentEngineMode = car->getEngineMode();

    if (this->previousEngineMode != currentEngineMode
        && currentEngineMode == StarCarEngineMode::CEM_On) {

        for (int index = 0; index < 5; index++) {
            this->SwitchLed(&this->forwardLedIsOn, true, this->forwardLedPin);
            this->SwitchLed(&this->backwardLedIsOn, true, this->backwardLedPin);
            this->SwitchLed(&this->rightFlashLedOn, true, this->rightFlashLedPin);
            this->SwitchLed(&this->leftFlashLedOn, true, this->leftFlashLedPin);
            delay(100);

            this->SwitchLed(&this->forwardLedIsOn, false, this->forwardLedPin);
            this->SwitchLed(&this->backwardLedIsOn, false, this->backwardLedPin);
            this->SwitchLed(&this->rightFlashLedOn, false, this->rightFlashLedPin);
            this->SwitchLed(&this->leftFlashLedOn, false, this->leftFlashLedPin);
            delay(100);
        }
    }

    if (currentEngineMode == StarCarEngineMode::CEM_On) {
        int8_t acceleration = car->getAcceleration();

        this->SwitchLed(&this->forwardLedIsOn, acceleration > 10, this->forwardLedPin);
        this->SwitchLed(&this->backwardLedIsOn, acceleration < -10, this->backwardLedPin);

        int8_t direction = car->getDirection();

        this->SwitchLed(&this->rightFlashLedOn, direction > 0, this->rightFlashLedPin);
        this->SwitchLed(&this->leftFlashLedOn, direction < 0, this->leftFlashLedPin);
    }

    this->previousEngineMode = currentEngineMode;
}

// ---------- Private methods ----------

void StarBoard::SwitchLed(bool* current, bool target, short pin)
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
