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
    this->mode = StarControllerMode::Hat;
    this->modeIsActive = false;

    this->xboxController = xboxController;
}

// ---------- Public properties ----------

StarControllerMode StarController::getMode()
{
    return this->mode;
}

const char* StarController::getName()
{
    return "Controller";
}

// ---------- Public methods ----------

void StarController::Task(StarCar* car)
{
    #if _DEBUG
    Serial.println("--> StarController::Task()");
    #endif

    if (this->xboxController->Xbox360Connected) {
        if (car->getMode() == StarCarMode::CarMode_Controller) {
            if (this->xboxController->getButtonClick(ButtonEnum::XBOX)) {
                if (this->mode == StarControllerMode::Hat)
                    this->mode = StarControllerMode::Button;
                else
                    this->mode = StarControllerMode::Hat;

                this->xboxController->setLedOff();
                this->modeIsActive = false;
            }

            if (!this->modeIsActive) {
                if (this->mode == StarControllerMode::Hat)
                    this->xboxController->setLedOn(LEDEnum::LED1);
                else
                    this->xboxController->setLedOn(LEDEnum::LED2);

                this->modeIsActive = true;
            }

            sbyte_t oldDirectionValue = car->getDirection();

            int_t leftX = this->xboxController->getAnalogHat(AnalogHatEnum::LeftHatX);
            sbyte_t newDirectionValue = ((float_t)leftX / 32768.0) * 100;

            if (oldDirectionValue != newDirectionValue) {
                car->setDirection(newDirectionValue);

                ////Serial.print("Direction: ");
                ////Serial.println(newDirectionValue);
            }

            if (this->mode == StarControllerMode::Hat) {
                sbyte_t oldSpeedValue = car->getSpeed();

                int_t rightY = this->xboxController->getAnalogHat(AnalogHatEnum::RightHatY);
                sbyte_t newSpeedValue = ((float_t)rightY / 32768.0) * 100;

                if (oldSpeedValue != newSpeedValue) {
                    car->setSpeed(newSpeedValue);

                    ////Serial.print("Speed: ");
                    ////Serial.println(newSpeedValue);
                }
            }
            else {
                sbyte_t oldSpeedValue = car->getDirection();

                int_t rightY = this->xboxController->getButtonPress(ButtonEnum::R2);
                rightY += -this->xboxController->getButtonPress(ButtonEnum::L2);

                sbyte_t newSpeedValue = ((float_t)rightY / 255.0) * 100;

                if (oldSpeedValue != newSpeedValue) {
                    car->setSpeed(newSpeedValue);

                    ////Serial.print("Speed: ");
                    ////Serial.println(newSpeedValue);
                }
            }

            if (this->xboxController->getButtonClick(ButtonEnum::X)) {
                StarCarEngineMode engineMode = car->getEngineMode();

                if (engineMode == StarCarEngineMode::CarEngineMode_Off) {
                    this->xboxController->setRumbleOn(100, 100);
                    delay(1000);
                    this->xboxController->setRumbleOn(0, 0);

                    car->setEngineMode(StarCarEngineMode::CarEngineMode_On);
                }
                else {
                    this->xboxController->setRumbleOn(50, 50);
                    delay(500);
                    this->xboxController->setRumbleOn(0, 0);

                    car->setEngineMode(StarCarEngineMode::CarEngineMode_Off);
                }
            }

            if (this->xboxController->getButtonClick(ButtonEnum::B))
                car->setEngineMode(StarCarEngineMode::CarEngineMode_Off);
        }
        else {
            this->xboxController->setLedOff();
        }
    }
}

// ---------- Protected methods ----------

byte_t StarController::InitCore()
{
    return StarControllerResult::ControllerResult_Success;
}
