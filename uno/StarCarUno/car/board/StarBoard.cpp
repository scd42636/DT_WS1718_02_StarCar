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

    this->previousEngineMode = StarCarEngineMode::CarEngineMode_Off;

    #if SERIAL_MODE == SERIAL_MODE_LIBRARY
    this->protocol = new ArduinoSerialProtocol(
        &Serial,
        (uint8_t*)&this->payload,
        sizeof(StarBoardExchangeData));
    #endif
}

// ---------- Public properties ----------

const char* StarBoard::getName()
{
    return "Board";
}

// ---------- Public methods ----------

void StarBoard::Task(StarCar* car)
{
    #if _DEBUG
    Serial.println("--> StarBoard::Task()");
    #endif

    #if SERIAL_MODE == SERIAL_MODE_ARDUINO
    if (Serial.available()) {
        int value = Serial.read();

        if (value >= 48 && value <= 57) {
            StarCarMode mode = (StarCarMode)(value - 48);
            car->setMode(mode);
        }
    }
    #elif SERIAL_MODE == SERIAL_MODE_IBC
    this->ibcDriver->next(car);
    #elif SERIAL_MODE == SERIAL_MODE_LIBRARY
    uint8_t receiveState = this->protocol->receive();

    if (receiveState == ProtocolState::SUCCESS) {
        car->setRequest(this->payload.Request);
        car->setMode(this->payload.Mode);
    }
    else {
        this->payload.Request = car->getRequest();
        this->payload.Mode = car->getMode();
    }

    if (car->IsRequested(StarCarSensorRequest::CarSensorRequest_Accelerator)) {
        this->payload.AccelerationXParity = EEPROM.read(EEPROM_ACCELEROMETER_X_PARITY);
        this->payload.AccelerationXValue = EEPROM.read(EEPROM_ACCELEROMETER_X_VALUE);
        this->payload.AccelerationYParity = EEPROM.read(EEPROM_ACCELEROMETER_Y_PARITY);
        this->payload.AccelerationYValue = EEPROM.read(EEPROM_ACCELEROMETER_Y_VALUE);
    }
    else {
        this->payload.AccelerationXParity = 0;
        this->payload.AccelerationXValue = 0;
        this->payload.AccelerationYParity = 0;
        this->payload.AccelerationYValue = 0;
    }

    if (car->IsRequested(StarCarSensorRequest::CarSensorRequest_Magnet)) {
        this->payload.DirectionParity = EEPROM.read(EEPROM_MAGNETOMETER_PARITY);
        this->payload.DirectionValue = EEPROM.read(EEPROM_MAGNETOMETER_VALUE);
    }
    else {
        this->payload.DirectionParity = 0;
        this->payload.DirectionValue = 0;
    }

    if (car->IsRequested(StarCarSensorRequest::CarSensorRequest_Sonic)) {
        this->payload.DistanceFront = EEPROM.read(EEPROM_SONIC_FRONT_VALUE);
        this->payload.DistanceBack = EEPROM.read(EEPROM_SONIC_BACK_VALUE);
    }
    else {
        this->payload.DistanceFront = 0;
        this->payload.DistanceBack = 0;
    }

    this->protocol->send();
    #endif

    StarCarEngineMode currentEngineMode = car->getEngineMode();

    if (this->previousEngineMode != currentEngineMode
        && currentEngineMode == StarCarEngineMode::CarEngineMode_On) {

        this->Blink(StarBoardLedPanels::BoardLedPanel_All, /*times:*/ 5, /*interval:*/ 100);
    }

    if (currentEngineMode == StarCarEngineMode::CarEngineMode_On) {
        if (car->IsBlocked()) {
            StarBoardLedPanels panels = StarBoardLedPanels::BoardLedPanel_All;

            if (car->IsFrontBlocked(/*exclusive*/ true))
                panels = StarBoardLedPanels::BoardLedPanel_Front;
            else if (car->IsBackBlocked(/*exclusive*/ true))
                panels = StarBoardLedPanels::BoardLedPanel_Back;

            this->Blink(panels, /*times:*/ 3, /*interval:*/ 50);
        }
        else {
            sbyte_t speed = car->getSpeed();

            this->SwitchLed(&this->frontLedIsOn, speed > 10, this->frontLedPin);
            this->SwitchLed(&this->backLedIsOn, speed < -10, this->backLedPin);

            sbyte_t direction = car->getDirection();

            this->SwitchLed(&this->rightLedOn, direction > 0, this->rightLedPin);
            this->SwitchLed(&this->leftLedOn, direction < 0, this->leftLedPin);
        }
    }

    this->previousEngineMode = currentEngineMode;
}

// ---------- Protected methods ----------

byte_t StarBoard::InitCore()
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

// ---------- Private methods ----------

void StarBoard::Blink(StarBoardLedPanels panels, short_t times, short_t interval)
{
    for (int_t index = 0; index < times; index++) {
        if ((panels & StarBoardLedPanels::BoardLedPanel_Front) == StarBoardLedPanels::BoardLedPanel_Front)
            this->SwitchLed(&this->frontLedIsOn, true, this->frontLedPin);

        if ((panels & StarBoardLedPanels::BoardLedPanel_Back) == StarBoardLedPanels::BoardLedPanel_Back)
            this->SwitchLed(&this->backLedIsOn, true, this->backLedPin);

        if ((panels & StarBoardLedPanels::BoardLedPanel_Right) == StarBoardLedPanels::BoardLedPanel_Right)
            this->SwitchLed(&this->rightLedOn, true, this->rightLedPin);

        if ((panels & StarBoardLedPanels::BoardLedPanel_Left) == StarBoardLedPanels::BoardLedPanel_Left)
            this->SwitchLed(&this->leftLedOn, true, this->leftLedPin);

        delay(interval);

        if ((panels & StarBoardLedPanels::BoardLedPanel_Front) == StarBoardLedPanels::BoardLedPanel_Front)
            this->SwitchLed(&this->frontLedIsOn, false, this->frontLedPin);

        if ((panels & StarBoardLedPanels::BoardLedPanel_Back) == StarBoardLedPanels::BoardLedPanel_Back)
            this->SwitchLed(&this->backLedIsOn, false, this->backLedPin);

        if ((panels & StarBoardLedPanels::BoardLedPanel_Right) == StarBoardLedPanels::BoardLedPanel_Right)
            this->SwitchLed(&this->rightLedOn, false, this->rightLedPin);

        if ((panels & StarBoardLedPanels::BoardLedPanel_Left) == StarBoardLedPanels::BoardLedPanel_Left)
            this->SwitchLed(&this->leftLedOn, false, this->leftLedPin);

        delay(interval);
    }
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
