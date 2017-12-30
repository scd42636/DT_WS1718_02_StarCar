//--------------------------------------------------------------------------------------------------
// <copyright file="StarCarUno.ino" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
// <author>Mehmet Billor</author>
//--------------------------------------------------------------------------------------------------

#include "./car/StarCar.h"
#include "./car/board/StarBoard.h"

#include "./car/actuators/StarMotor.h"
#include "./car/actuators/StarServo.h"

#include "./car/sensors/StarAccelerometer.h"
#include "./car/sensors/StarMagnetometer.h"
#include "./car/sensors/StarSonic.h"

#include "./car/controllers/StarController.h"
#include "./car/controllers/StarWatch.h"

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

IbcDriver ibcDriver(115200);

#define Board_FrontLedPin           6
#define Board_BackLedPin            7
#define Board_LeftFlashLedPin       PIN_DISCONNECTED
#define Board_RightFlashLedPin      PIN_DISCONNECTED
StarBoard board(&ibcDriver, Board_FrontLedPin, Board_BackLedPin, Board_LeftFlashLedPin, Board_RightFlashLedPin);

StarController controller(&xboxController);
StarWatch watch(&usbController);

#define Motor_RxPin                 2
#define Motor_TxPin                 3
#define Motor_ResetPin              4
StarMotor motor(Motor_RxPin, Motor_TxPin, Motor_ResetPin);

#define Servo_StepPin               5
StarServo servo(Servo_StepPin);

StarAccelerometer accelerometer;
StarMagnetometer magnetometer;

#define SonicFront_Pin              12
StarSonic sonicFront(SonicFront_Pin, StarSonicLocation::SonicLocation_Front);

#define SonicBack_Pin               13
StarSonic sonicBack(SonicBack_Pin, StarSonicLocation::SonicLocation_Back);


StarCarModule* modules[] = {
    &board,
    //&accelerometer,
    //&magnetometer,
    //&sonicBack,
    //&sonicFront,
    //&controller,
    &watch,
    &servo,
    &motor
};

StarCar car(modules, sizeof(modules) / sizeof(StarCarModule*));


void setup()
{
    #if _DEBUG
    Serial.println("----------");
    Serial.println("-> setup()");
    #endif

    usb.Init();
    //delay(200);
    car.Init();

    #if _DEBUG
    Serial.println("----------");
    #endif

    //car.setMode(StarCarMode::CarMode_Controller);
    car.setMode(StarCarMode::CarMode_Watch);

    //car.setRequest(StarCarSensorRequest::CSR_Sonic);
}

void loop()
{
    #if _DEBUG
    Serial.println("----------");
    Serial.println("-> loop()");
    #endif

    usb.Task();
    car.Task();

    #if _DEBUG
    Serial.println("----------");
    #endif
}
