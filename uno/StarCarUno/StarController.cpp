//--------------------------------------------------------------------------------------------------
// <copyright file="StarController.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "StarController.h"


// ---------- Public constructors ----------

StarController::StarController(XBoxController* xboxController)
{
    this->xboxController = xboxController;
}

// ---------- Public methods ----------

StarControllerResult StarController::Init()
{
    return StarControllerResult::CR_Success;
}

void StarController::Task(StarCar* car)
{
    #if _DEBUG
    Serial.println("--> StarController::Task()");
    #endif

    if (this->xboxController->Xbox360Connected) {
        if (car->getMode() == StarCarMode::CM_Controller) {
            this->xboxController->setLedMode(ALTERNATING);

            int16_t leftX = this->xboxController->getAnalogHat(LeftHatX);
            car->setDirection(((float)leftX / 32768) * 100);

            //Serial.print("Controller: Direction = ");
            //Serial.println((float)leftX / 32768);

            int16_t rightY = this->xboxController->getAnalogHat(RightHatY);
            car->setAcceleration(((float)rightY / 32768) * 100);

            //Serial.print("Controller: Acceleration = ");
            //Serial.println((float)rightY / 32768);

            if (this->xboxController->getButtonClick(X)) {
                Serial.println("X");
                StarCarEngineMode engineMode = car->getEngineMode();

                if (engineMode == StarCarEngineMode::CEM_Off) {
                    this->xboxController->setRumbleOn(100, 100);
                    delay(1000);
                    this->xboxController->setRumbleOn(0, 0);

                    car->setEngineMode(StarCarEngineMode::CEM_On);
                }
                else {
                    this->xboxController->setRumbleOn(50, 50);
                    delay(500);
                    this->xboxController->setRumbleOn(0, 0);

                    car->setEngineMode(StarCarEngineMode::CEM_Off);
                }
            }

            if (this->xboxController->getButtonClick(B)) {
                car->setEngineMode(StarCarEngineMode::CEM_Off);
                Serial.println("B");
            }
        }
        else {
            this->xboxController->setLedOff();
        }
    }

    ////if (this->xboxController->getButtonPress(L2)
    ////    || this->xboxController->getButtonPress(R2)) {
    ////    #if TEST
    ////    Serial.print("L2: ");
    ////    Serial.print(this->xboxController->getButtonPress(L2));

    ////    Serial.print("\tR2: ");
    ////    Serial.println(this->xboxController->getButtonPress(R2));
    ////    #endif

    ////    this->xboxController->setRumbleOn(
    ////        this->xboxController->getButtonPress(L2),
    ////        this->xboxController->getButtonPress(R2));
    ////}
    ////else {
    ////    this->xboxController->setRumbleOn(0, 0);
    ////}

    ////if (this->xboxController->getAnalogHat(LeftHatX) > 7500
    ////    || this->xboxController->getAnalogHat(LeftHatX) < -7500
    ////    || this->xboxController->getAnalogHat(LeftHatY) > 7500
    ////    || this->xboxController->getAnalogHat(LeftHatY) < -7500
    ////    || this->xboxController->getAnalogHat(RightHatX) > 7500
    ////    || this->xboxController->getAnalogHat(RightHatX) < -7500
    ////    || this->xboxController->getAnalogHat(RightHatY) > 7500
    ////    || this->xboxController->getAnalogHat(RightHatY) < -7500) {
    ////    if (this->xboxController->getAnalogHat(LeftHatX) > 7500
    ////        || this->xboxController->getAnalogHat(LeftHatX) < -7500) {
    ////        #if TEST
    ////        Serial.print(F("LeftHatX: "));
    ////        Serial.print(this->xboxController->getAnalogHat(LeftHatX));
    ////        Serial.print("\t");
    ////        #endif
    ////    }
    ////    if (this->xboxController->getAnalogHat(LeftHatY) > 7500
    ////        || this->xboxController->getAnalogHat(LeftHatY) < -7500) {
    ////        #if TEST
    ////        Serial.print(F("LeftHatY: "));
    ////        Serial.print(this->xboxController->getAnalogHat(LeftHatY));
    ////        Serial.print("\t");
    ////        #endif
    ////    }
    ////    if (this->xboxController->getAnalogHat(RightHatX) > 7500
    ////        || this->xboxController->getAnalogHat(RightHatX) < -7500) {
    ////        #if TEST
    ////        Serial.print(F("RightHatX: "));
    ////        Serial.print(this->xboxController->getAnalogHat(RightHatX));
    ////        Serial.print("\t");
    ////        #endif
    ////    }
    ////    if (this->xboxController->getAnalogHat(RightHatY) > 7500
    ////        || this->xboxController->getAnalogHat(RightHatY) < -7500) {
    ////        #if TEST
    ////        Serial.print(F("RightHatY: "));
    ////        Serial.print(this->xboxController->getAnalogHat(RightHatY));
    ////        #endif
    ////    }
    ////    #if TEST
    ////    Serial.println();
    ////    #endif
    ////}

    ////if (this->xboxController->getButtonClick(UP)) {
    ////    this->xboxController->setLedOn(LED1);
    ////    #if TEST
    ////    Serial.println(F("Up"));
    ////    #endif
    ////}
    ////if (this->xboxController->getButtonClick(DOWN)) {
    ////    this->xboxController->setLedOn(LED4);
    ////    #if TEST
    ////    Serial.println(F("Down"));
    ////    #endif
    ////}
    ////if (this->xboxController->getButtonClick(LEFT)) {
    ////    this->xboxController->setLedOn(LED3);
    ////    #if TEST
    ////    Serial.println(F("Left"));
    ////    #endif
    ////}
    ////if (this->xboxController->getButtonClick(RIGHT)) {
    ////    this->xboxController->setLedOn(LED2);
    ////    #if TEST
    ////    Serial.println(F("Right"));
    ////    #endif
    ////}

    ////if (this->xboxController->getButtonClick(START)) {
    ////    this->xboxController->setLedMode(ALTERNATING);
    ////    #if TEST
    ////    Serial.println(F("Start"));
    ////    #endif
    ////}
    ////if (this->xboxController->getButtonClick(BACK)) {
    ////    this->xboxController->setLedBlink(ALL);
    ////    #if TEST
    ////    Serial.println(F("Back"));
    ////    #endif
    ////}
    ////if (this->xboxController->getButtonClick(L3)) {
    ////    #if TEST
    ////    Serial.println(F("L3"));
    ////    #endif
    ////}
    ////if (this->xboxController->getButtonClick(R3)) {
    ////    #if TEST
    ////    Serial.println(F("R3"));
    ////    #endif
    ////}

    ////if (this->xboxController->getButtonClick(L1)) {
    ////    #if TEST
    ////    Serial.println(F("L1"));
    ////    #endif
    ////}
    ////if (this->xboxController->getButtonClick(R1)) {
    ////    #if TEST
    ////    Serial.println(F("R1"));
    ////    #endif
    ////}
    ////if (this->xboxController->getButtonClick(XBOX)) {
    ////    this->xboxController->setLedMode(ROTATING);
    ////    #if TEST
    ////    Serial.println(F("Xbox"));
    ////    #endif
    ////}

    ////if (this->xboxController->getButtonClick(A)) {
    ////    #if TEST
    ////    Serial.println(F("A"));
    ////    #endif
    ////}
    ////if (this->xboxController->getButtonClick(B)) {
    ////    #if TEST
    ////    Serial.println(F("B"));
    ////    #endif
    ////}
    ////if (this->xboxController->getButtonClick(X)) {
    ////    #if TEST
    ////    Serial.println(F("X"));
    ////    #endif
    ////}
    ////if (this->xboxController->getButtonClick(Y)) {
    ////    #if TEST
    ////    Serial.println(F("Y"));
    ////    #endif
    ////}

    delay(1);
}
