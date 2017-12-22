//--------------------------------------------------------------------------------------------------
// <copyright file="StarCarUno.ino" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
// <author>Mehmet Billor</author>
//--------------------------------------------------------------------------------------------------

#include "StarCar.h"
#include "StarBoard.h"
#include "StarController.h"
#include "StarMotor.h"
#include "StarServo.h"
#include "StarSonic.h"
#include "StarWatch.h"

// REASONs WHY THE FIRMWARE COULD STOP WORKING:
// - Arduino classes initializes using new. Do not create heap variables of them.
// - Serial.print / Serial.println: If used to extensive an internal overflow will occur.
// - Using the PIN #10 did not work for the servo control class.

// USING THE USB SHIELD THE FOLLOWING PIN BEHAIVOUR APPLIES TO THE BOARD:
// - Working PINs: 2, 3, 4, 5, 6, 7, 10, 12, 13
// - Strange working PINs: 8
// - Not working PINs: 0, 1, 9, 11

Usb usb;
UsbDriver usbDriver;
UsbController usbController(&usb, &usbDriver);
XBoxController xboxController(&usb);

StarCar car;

#define Board_FrontLedPin           6
#define Board_BackLedPin            7
#define Board_LeftFlashLedPin       PIN_DISCONNECTED
#define Board_RightFlashLedPin      PIN_DISCONNECTED
StarBoard board(Board_FrontLedPin, Board_BackLedPin, Board_LeftFlashLedPin, Board_RightFlashLedPin);

StarController controller(&xboxController);
StarWatch watch(&usbController);

#define Motor_RxPin                 2
#define Motor_TxPin                 3
#define Motor_ResetPin              4
StarMotor motor(Motor_RxPin, Motor_TxPin, Motor_ResetPin);

#define Servo_StepPin               5
StarServo servo(Servo_StepPin);

#define SonicFront_Pin              12
StarSonic sonicFront(SonicFront_Pin, StarSonicLocation::SCL_Front);

#define SonicBack_Pin               13
StarSonic sonicBack(SonicBack_Pin, StarSonicLocation::SCL_Back);


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
    Serial.print("--> Initializing Sonic (back)...\t\t");
    #endif

    if (sonicBack.Init() == StarSonicResult::SCR_Success) {
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
    Serial.print("--> Initializing Sonic (front)...\t\t");
    #endif

    if (sonicFront.Init() == StarSonicResult::SCR_Success) {
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

    car.setMode(StarCarMode::CM_Controller);
    //car.setRequest(StarCarSensorRequest::CSR_Sonic);
}

void loop()
{
    #if _DEBUG
    Serial.println("----------");
    Serial.println("-> loop()");
    #endif

    usb.Task();
    board.Task(&car);

    //sonicFront.Task(&car);
    //sonicBack.Task(&car);

    controller.Task(&car);
    // watch.Task(&car);
    
    motor.Task(&car);
    servo.Task(&car);

    #if _DEBUG
    Serial.println("----------");
    #endif
}
