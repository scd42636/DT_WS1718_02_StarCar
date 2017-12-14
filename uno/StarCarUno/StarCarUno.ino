//--------------------------------------------------------------------------------------------------
// <copyright file="StarCarUno.ino" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "StarCar.h"
#include "StarBoard.h"
#include "StarController.h"
#include "StarMotor.h"
#include "StarServo.h"
#include "StarWatch.h"

// REASONs WHY THE FIRMWARE COULD STOP WORKING:
// - Arduino classes initializes using new. Do not create heap variables of them.
// - Serial.print / Serial.println: If used to extensive an internal overflow will occur.
// - Using the PIN #10 did not work for the servo control class.

static Usb usb;
static UsbDriver usbDriver;
static UsbController usbController(&usb, &usbDriver);
static XBoxController xboxController(&usb);

static StarCar car;

static StarBoard board;
static StarController controller(&xboxController);
static StarWatch watch(&usbController);

#define Motor_RxPin         11
#define Motor_TxPin         12
#define Motor_ResetPin      13
static StarMotor motor(Motor_RxPin, Motor_TxPin, Motor_ResetPin);

#define Servo_StepPin       5
static StarServo servo(Servo_StepPin);

void setup()
{
    Serial.begin(115200);

    #if _DEBUG
    Serial.println("----------");
    Serial.println("-> setup()");
    #endif

    if (usb.Init() == -1) {
        #if _DEBUG
        Serial.println("--> Initializing USB Shield...\t\tFAILED!");
        #endif
    }

    delay(200);

    #if _DEBUG
    delay(100);
    Serial.print("--> Initializing Board...\t\t");
    #endif

    if (board.Init() == StarBoardResult::BR_Success) {
        #if _DEBUG
        Serial.println("success!");
        #endif
    }
    else {
        #if _DEBUG
        Serial.println("FAILED!");
        #endif
    }

    #if _DEBUG
    delay(100);
    Serial.print("--> Initializing Controller...\t\t");
    #endif

    if (controller.Init() == StarControllerResult::CR_Success) {
        #if _DEBUG
        Serial.println("success!");
        #endif
    }
    else {
        #if _DEBUG
        Serial.println("FAILED!");
        #endif
    }

    #if _DEBUG
    delay(100);
    Serial.print("--> Initializing Watch...\t\t");
    #endif

    if (watch.Init() == StarWatchResult::WR_Success) {
        #if _DEBUG
        Serial.println("success!");
        #endif
    }
    else {
        #if _DEBUG
        Serial.println("FAILED!");
        #endif
    }

    #if _DEBUG
    delay(100);
    Serial.print("--> Initializing Servo...\t\t");
    #endif

    if (servo.Init() == StarServoResult::SR_Success) {
        #if _DEBUG
        Serial.println("success!");
        #endif
    }
    else {
        #if _DEBUG
        Serial.println("FAILED!");
        #endif
    }

    #if _DEBUG
    delay(100);
    Serial.print("--> Initializing Motor...\t\t");
    #endif

    if (motor.Init() == StarMotorResult::MR_Success) {
        #if _DEBUG
        Serial.println("success!");
        #endif
    }
    else {
        #if _DEBUG
        Serial.println("FAILED!");
        #endif
    }

    #if _DEBUG
    Serial.println("----------");
    #endif

    //car.setMode(StarCarMode::CM_Controller);
}

void loop()
{
    #if _DEBUG
    Serial.println("----------");
    Serial.println("-> loop()");
    #endif

    usb.Task();
    board.Task(&car);
    controller.Task(&car);
    watch.Task(&car);
    motor.Task(&car);
    servo.Task(&car);

    //motor.Test01();
    //servo.Test02();

    #if _DEBUG
    Serial.println("----------");
    #endif

    delay(1);
}
