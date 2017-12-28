//--------------------------------------------------------------------------------------------------
// <copyright file="StarBoard.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "StarBoard.h"


// ---------- Public constructors ----------

StarBoard::StarBoard(
    pin_t frontLedPin,
    pin_t backLedPin,
    pin_t leftLedPin,
    pin_t rightLedPin)
{
    this->frontLedPin = frontLedPin;
    this->frontLedIsOn = false;

    this->backLedPin = backLedPin;
    this->backLedIsOn = false;

    this->leftLedPin = leftLedPin;
    this->leftLedOn = false;

    this->rightLedPin = rightLedPin;
    this->rightLedOn = false;

    this->previousEngineMode = StarCarEngineMode::CarEngineMode_Off;
}

// ---------- Public methods ----------

StarBoardResult StarBoard::Init()
{
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

    return StarBoardResult::BoardResult_Success;
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
        && currentEngineMode == StarCarEngineMode::CarEngineMode_On) {

        for (int_t index = 0; index < 5; index++) {
            this->SwitchLed(&this->frontLedIsOn, true, this->frontLedPin);
            this->SwitchLed(&this->backLedIsOn, true, this->backLedPin);
            this->SwitchLed(&this->rightLedOn, true, this->rightLedPin);
            this->SwitchLed(&this->leftLedOn, true, this->leftLedPin);
            delay(100);

            this->SwitchLed(&this->frontLedIsOn, false, this->frontLedPin);
            this->SwitchLed(&this->backLedIsOn, false, this->backLedPin);
            this->SwitchLed(&this->rightLedOn, false, this->rightLedPin);
            this->SwitchLed(&this->leftLedOn, false, this->leftLedPin);
            delay(100);
        }
    }

    if (currentEngineMode == StarCarEngineMode::CarEngineMode_On) {
        int8_t speed = car->getSpeed();

        this->SwitchLed(&this->frontLedIsOn, speed > 10, this->frontLedPin);
        this->SwitchLed(&this->backLedIsOn, speed < -10, this->backLedPin);

        int8_t direction = car->getDirection();

        this->SwitchLed(&this->rightLedOn, direction > 0, this->rightLedPin);
        this->SwitchLed(&this->leftLedOn, direction < 0, this->leftLedPin);
    }

    this->previousEngineMode = currentEngineMode;
}

// ---------- Private methods ----------

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
