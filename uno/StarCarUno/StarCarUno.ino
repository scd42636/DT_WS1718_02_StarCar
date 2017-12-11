//--------------------------------------------------------------------------------------------------
// <copyright file="StarCarUno.ino" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "./driver/UsbDriver.h"

#include "StarCar.h"
#include "StarBoard.h"
#include "StarMotor.h";
#include "StarServo.h";
#include "StarWatch.h";

static StarBoard board = StarBoard();

#define Motor_RxPin         2
#define Motor_TxPin         3
#define Motor_ResetPin      4

static StarMotor motor = StarMotor(Motor_RxPin, Motor_TxPin, Motor_ResetPin);

#define Servo_StepPin       10

static StarServo servo = StarServo(Servo_StepPin);

Usb usb;
UsbDriver usbDriver;
UsbController usbController(&usb, &usbDriver);

static StarWatch watch = StarWatch(&usbController);


/// <summary>
// Runs once when reset is pressed or the board is powered.
/// </summary>
void setup()
{
    Serial.begin(115200);
    delay(100);

    #if _DEBUG
    Serial.println("----------");
    Serial.println("-> setup()");
    #endif
    
    if (usb.Init() == -1)
        Serial.println("--> Initializing USB Shield...\t\tFAILED!");

    delay(200);
    Serial.print("--> Initializing Board...\t\t");

    if (board.Init() == StarBoardResult::BR_Success)
        Serial.println("success!");
    else
        Serial.println("FAILED!");

    delay(100);
    Serial.print("--> Initializing Motor...\t\t");

    if (motor.Init() == StarMotorResult::MR_Success)
        Serial.println("success!");
    else
        Serial.println("FAILED!");

    delay(100);
    Serial.print("--> Initializing Servo...\t\t");

    if (servo.Init() == StarServoResult::SR_Success)
        Serial.println("success!");
    else
        Serial.println("FAILED!");

    delay(100);
    Serial.print("--> Initializing Watch...\t\t");

    if (watch.Init() == StarWatchResult::WR_Success)
        Serial.println("success!");
    else
        Serial.println("FAILED!");

    delay(100);

    #if _DEBUG
    Serial.println("----------");
    #endif
}

/// <summary>
// Runs continues until the board is powered off or reset.
/// </summary>
void loop()
{
    usb.Task();

    if (!usbController.isReady())
        return;

    #if _DEBUG
    Serial.println("----------");
    Serial.println("-> loop()");
    #endif

    //servo.Test02();

    board.Task();
    delay(100);

    motor.Task();
    delay(100);

    servo.Task();
    delay(100);

    watch.Task();
    delay(100);

    #if _DEBUG
    Serial.println("----------");
    #endif
}
