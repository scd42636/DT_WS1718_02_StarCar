//--------------------------------------------------------------------------------------------------
// <copyright file="Servo.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

// Implemented using the library code provided on:
// - https://github.com/laurb9/StepperDriver/blob/master/src/BasicStepperDriver.h
// - https://github.com/laurb9/StepperDriver/blob/master/src/BasicStepperDriver.cpp

#pragma once
#define STEP_PULSE(steps, microsteps, rpm) (60*1000000L/steps/microsteps/rpm)

enum ServoDirection
{
    SD_Forward = HIGH,
    SD_Backward = LOW
};

enum ServoState
{
    SM_Sopped = 0,
    SM_Accelerating = 1,
    SM_Cruising = 2,
    SM_Decelerating = 3
};

enum ServoSpeedProfile
{
    SSP_Constant = 0,
    SSP_Linear = 1
};

// see A4988.pdf page 6
enum ServoMicrosteps
{
    SSR_Full = 0b000,
    SSR_Half = 0b001,
    SSR_Quarter = 0b010,
    SSR_Eighth = 0b011,
    SSR_Sixteenth = 0b111
};

enum ServoResult
{
    SR_Success = 0,

    SR_Failed = -1,
    SR_MicrostepsIsOutOfRange = -2
};

class Servo
{
    // ---------- Public const fields ----------
public:
    const ServoMicrosteps MinMicrosteps = ServoMicrosteps::SSR_Full;
    const ServoMicrosteps MaxMicrosteps = ServoMicrosteps::SSR_Sixteenth;

    const short DefaultRpm = 60;

    const short DefaultAcceleration = 1000;
    const short DefaultDeceleration = 1000;

    // ---------- Private fields ----------
private:
    bool isEnabled = false;

    short directionPin = 0;
    short enablePin = 0;
    short stepPin = 0;
    short ms1Pin = 0;
    short ms2Pin = 0;
    short ms3Pin = 0;

    short acceleration = 0;
    short deceleration = 0;

    short rpm = 0;
    ServoDirection direction = ServoDirection::SD_Forward;
    ServoSpeedProfile speedProfile = ServoSpeedProfile::SSP_Constant;



    // ===
    long rest = 0;
    unsigned long last_action_end = 0;
    unsigned long next_action_interval = 0;

    // motor steps per revolution (usually 200)
    short motor_steps = 0;

    short dir_pin = -1;
    short step_pin = -1;
    short enable_pin = -1;

    // current microstep level (1,2,4,8,...), must be < getMaxMicrostep()
    ServoMicrosteps microsteps = ServoMicrosteps::SSR_Full;

    // tWH(STEP) pulse duration, STEP high, min value (us)
    // tA STEP minimum, HIGH pulse width (1us)
    static const int step_high_min = 1;

    // tWL(STEP) pulse duration, STEP low, min value (us)
    // tB STEP minimum, LOW pulse width (1us)
    static const int step_low_min = 1;

    // tWAKE wakeup time, nSLEEP inactive to STEP (us)
    // wakeup time, nSLEEP inactive to STEP (1000us)
    static const int wakeup_time = 1000;

    short rpm = 0;

    long step_count = 0;        // current position
    long steps_remaining = 0;   // to complete the current move (absolute value)
    long steps_to_cruise = 0;   // steps to reach cruising (max) rpm
    long steps_to_brake = 0;    // steps needed to come to a full stop
    long step_pulse = 0;        // step pulse duration (microseconds)
    // ===






    // ---------- Public constructors ----------
public:
    Servo(
        short directionPin,
        short stepPin,
        short enablePin,
        short ms1Pin,
        short ms2Pin,
        short ms3Pin);

    // ---------- Public properties ----------
public:
    short getAcceleration();
    ServoResult setAcceleration(short value);

    short getDeceleration();
    ServoResult setDeceleration(short value);

    ServoDirection getDirection();
    ServoResult setDirection(ServoDirection value);

    short getCurrentRpm();

    bool getEnabled();
    ServoResult setEnabled(bool value);

    ServoMicrosteps getMicrosteps();
    ServoResult setMicrosteps(ServoMicrosteps value);

    short getRpm();
    ServoResult setRpm(short value);

    ServoState getState();

    short getSteps();

    ServoSpeedProfile getSpeedProfile();
    ServoResult setSpeedProfile(ServoSpeedProfile value);

    // ---------- Public methods ----------
public:
    void BeginBrake();
    void EndBrake();

    long CalculateTimeToMove(long steps);

    void BeginMove(long steps);
    void EndMove();

    void BeginRotate(long degree);
    void EndRotate();

    void Move(long steps);
    void Rotate(long degree);

    ServoResult Enable();
    ServoResult Disable();

    ServoResult Setup();

    // ---------- Private methods ----------
private:
    long CalculateSteps(long degree);
    void CalculateStepPulse();

    void UpdateMove(long steps);

    long YieldMove();
};