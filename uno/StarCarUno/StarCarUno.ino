//--------------------------------------------------------------------------------------------------
// <copyright file="StarCarUno.ino" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "Motor.h";
#include "Servo.h";

#define Motor_RxPin 3       // Pin 3 connects to SMC TX
#define Motor_TxPin 4       // Pin 4 connects to SMC RX
#define Motor_ResetPin 5    // Pin 5 connects to SMC nRST
#define Motor_ErrorPin 6    // Pin 6 connects to SMC ERR

#define Servo_DirPin -1
#define Servo_StepPin -1
#define Servo_EnablePin -1
#define Servo_MS1Pin -1
#define Servo_MS2Pin -1
#define Servo_MS3Pin -1

static Motor motor = Motor(Motor_RxPin, Motor_TxPin, Motor_ResetPin, Motor_ErrorPin);
static Servo servo = Servo(Servo_DirPin, Servo_StepPin, Servo_EnablePin, Servo_MS1Pin, Servo_MS2Pin, Servo_MS3Pin);


void setupBoard()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void setupMotor()
{
    motor.Setup();

    motor.ChangeLimit(MotorLimit::ML_MaxAccelerationForward, 1);
    motor.ChangeLimit(MotorLimit::ML_MaxAccelerationBackward, 1);
    motor.ChangeLimit(MotorLimit::ML_MaxDeceleration, 10);
}

void setupServo()
{
    servo.Setup();
}

/// <summary>
// Runs once when reset is pressed or the board is powered.
/// </summary>
void setup()
{
    setupBoard();
    setupMotor();
    setupServo();
}

void loop()
{
    motor.ChangeSpeed(300);

    int currentSpeed = motor.ReadCurrentSpeed();
    int targetSpeed = motor.ReadTargetSpeed();

    Serial.print("Target Speed: ");
    Serial.println(targetSpeed);

    Serial.print("Current Speed: ");
    Serial.println(currentSpeed);

    Serial.println("-----");
}
