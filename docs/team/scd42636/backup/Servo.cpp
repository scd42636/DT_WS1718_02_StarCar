//--------------------------------------------------------------------------------------------------
// <copyright file="Servo.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "Servo.h"


// ---------- Public constructors ----------

Servo::Servo(
    short directionPin,
    short stepPin,
    short enablePin = PIN_DISCONNECTED,
    short ms1Pin = PIN_DISCONNECTED,
    short ms2Pin = PIN_DISCONNECTED,
    short ms3Pin = PIN_DISCONNECTED)
{
    this->directionPin = directionPin;
    this->stepPin = stepPin;
    this->enablePin = enablePin;

    this->ms1Pin = ms1Pin;
    this->ms2Pin = ms2Pin;
    this->ms3Pin = ms3Pin;

    this->rpm = DefaultRpm;
    this->acceleration = DefaultAcceleration;
    this->deceleration = DefaultDeceleration;
}

// ---------- Public properties ----------

short Servo::getAcceleration()
{
    return this->acceleration;
}
ServoResult Servo::setAcceleration(short value)
{
    this->acceleration = value;
    return ServoResult::SR_Success;
}

short Servo::getCurrentRpm()
{
    return (short)(60 * 1000000L / this->step_pulse / this->microsteps / this->motor_steps);
}

short Servo::getDeceleration()
{
    return this->deceleration;
}
ServoResult Servo::setDeceleration(short value)
{
    this->deceleration = value;
    return ServoResult::SR_Success;
}

ServoDirection Servo::getDirection()
{
    return this->direction;
}
ServoResult Servo::setDirection(ServoDirection value)
{
    this->direction = value;
}

bool Servo::getEnabled()
{
    if (this->enablePin != PIN_DISCONNECTED)
        return digitalRead(this->enablePin) == HIGH;

    return false;
}
ServoResult Servo::setEnabled(bool value)
{
    #if _DEBUG
    Serial.println("-> Servo::setEnabled()");
    Serial.print("--> value=");
    Serial.println(value);
    #endif

    if (this->enablePin != PIN_DISCONNECTED) {
        if (value) {
            digitalWrite(this->enablePin, LOW);

            #if _DEBUG
            Serial.println("--> ENABLED");
            #endif
        }
        else {
            digitalWrite(this->enablePin, HIGH);

            #if _DEBUG
            Serial.println("--> DISABLED");
            #endif
        }
    }

    #if _DEBUG
    Serial.println("<- Servo::setEnabled()");
    #endif

    return ServoResult::SR_Success;
}

ServoMicrosteps Servo::getMicrosteps()
{
    int ms3 = digitalRead(this->ms3Pin);
    int ms2 = digitalRead(this->ms2Pin);
    int ms1 = digitalRead(this->ms1Pin);

    return (ServoMicrosteps)(ms3 | ms2 | ms1);
}
ServoResult Servo::setMicrosteps(ServoMicrosteps value)
{
    if (value < MinMicrosteps || value > MaxMicrosteps)
        return ServoResult::SR_MicrostepsIsOutOfRange;

    digitalWrite(this->ms3Pin, value & 4);
    digitalWrite(this->ms2Pin, value & 2);
    digitalWrite(this->ms1Pin, value & 1);

    return ServoResult::SR_Success;
}

// 1-200
short Servo::getRpm()
{
    return this->rpm;
}

ServoResult Servo::setRpm(short value)
{
    this->rpm = rpm;
}

ServoState Servo::getState()
{
    if (this->steps_remaining > 0) {
        if (this->steps_remaining <= this->steps_to_brake)
            return ServoState::SM_Decelerating;

        if (this->step_count <= this->steps_to_cruise)
            return ServoState::SM_Accelerating;

        return ServoState::SM_Cruising;
    }

    return ServoState::SM_Sopped;
}

ServoSpeedProfile Servo::getSpeedProfile()
{
    return this->speedProfile;
}

ServoResult Servo::setSpeedProfile(ServoSpeedProfile value)
{
    this->speedProfile = value;
    return ServoResult::SR_Success;
}

short Servo::getSteps()
{
    return this->motor_steps;
}

// ---------- Public methods ----------

void Servo::BeginBrake()
{
    switch (this->getState()) {
        // this applies to both SSP_Constant and SSP_Linear modes
        case ServoState::SM_Cruising:
            this->steps_remaining = this->steps_to_brake;
            break;

        case ServoState::SM_Accelerating:
            this->steps_remaining = this->step_count * this->acceleration / this->deceleration;
            break;

        default:
            break; // nothing to do if already stopped or braking
    }
}

void Servo::EndBrake()
{
    this->steps_remaining = 0;
}

long Servo::CalculateTimeToMove(long steps)
{
    long time = 0;

    switch (this->speedProfile) {
        case ServoSpeedProfile::SSP_Linear:
            this->BeginMove(steps);

            time = sqrt(2 * this->steps_to_cruise / this->acceleration)
                + (this->steps_remaining - this->steps_to_cruise - this->steps_to_brake)
                * STEP_PULSE(this->rpm, this->motor_steps, this->microsteps)
                + sqrt(2 * this->steps_to_brake / this->deceleration);

            break;

        case ServoSpeedProfile::SSP_Constant:
        default:
            time = STEP_PULSE(this->rpm, this->motor_steps, this->microsteps);
    }

    return time;
}

void Servo::BeginMove(long steps)
{
    #if _DEBUG
    Serial.println("-> Servo::BeginMove()");
    #endif

    long speed = 0;

    if (this->steps_remaining) {
        this->UpdateMove(steps);
    }
    else {
        if (steps < 0)
            this->direction = ServoDirection::SD_Backward;
        else
            this->direction = ServoDirection::SD_Forward;

        this->last_action_end = 0;
        this->steps_remaining = abs(steps);
        this->step_count = 0;
        this->rest = 0;

        switch (this->speedProfile) {
            case ServoSpeedProfile::SSP_Linear:
                // speed is in [steps/s]
                speed = this->rpm * this->motor_steps / 60;

                // how many steps from 0 to target rpm
                this->steps_to_cruise = speed * speed * this->microsteps / (2 * this->acceleration);

                // how many steps are needed from target rpm to a full stop
                this->steps_to_brake = this->steps_to_cruise * this->acceleration / this->deceleration;

                if (this->steps_remaining < this->steps_to_cruise + this->steps_to_brake) {
                    // cannot reach max speed, will need to brake early
                    this->steps_to_cruise = this->steps_remaining * this->deceleration / (this->acceleration + this->deceleration);
                    this->steps_to_brake = this->steps_remaining - this->steps_to_cruise;
                }

                // Initial pulse (c0) including error correction factor 0.676 [us]
                step_pulse = (1e+6) * 0.676 * sqrt(2.0f / (this->acceleration * this->microsteps));
                break;

            case ServoSpeedProfile::SSP_Constant:
            default:
                this->step_pulse = STEP_PULSE(this->rpm, this->motor_steps, this->microsteps);
                this->steps_to_cruise = 0;
                this->steps_to_brake = 0;
        }
    }

    #if _DEBUG
    Serial.println("<- Servo::BeginMove()");
    #endif
}

void Servo::EndMove()
{
    this->steps_remaining = 0;
}

void Servo::BeginRotate(long degree)
{
    this->BeginMove(this->CalculateSteps(degree));
}

void Servo::EndRotate()
{
    this->steps_remaining = 0;
}

void Servo::Move(long steps)
{
    #if _DEBUG
    Serial.println("-> Servo::Move()");
    #endif

    this->BeginMove(steps);
    while (this->YieldMove());

    #if _DEBUG
    Serial.println("<- Servo::Move()");
    #endif
}

void Servo::Rotate(long degree)
{
    #if _DEBUG
    Serial.println("-> Servo::Rotate()");
    #endif

    this->Move(this->CalculateSteps(degree));

    #if _DEBUG
    Serial.println("<- Servo::Rotate()");
    #endif
}

ServoResult Servo::Enable()
{
    return this->setEnabled(true);
}

ServoResult Servo::Disable()
{
    return this->setEnabled(false);
}

ServoResult Servo::Setup()
{
    #if _DEBUG
    Serial.println("-> Servo::Setup()");

    Serial.print("--> directionPin=");
    Serial.println(directionPin);
    Serial.print("--> stepPin=");
    Serial.println(stepPin);
    Serial.print("--> enablePin=");
    Serial.println(enablePin);
    #endif

    pinMode(this->directionPin, OUTPUT);
    digitalWrite(this->directionPin, HIGH);

    pinMode(this->stepPin, OUTPUT);
    digitalWrite(this->stepPin, LOW);

    if (this->enablePin != PIN_DISCONNECTED) {
        pinMode(this->enablePin, OUTPUT);
        digitalWrite(this->enablePin, HIGH); // disable
    }

    if (this->ms1Pin != PIN_DISCONNECTED
        && this->ms2Pin != PIN_DISCONNECTED
        && this->ms3Pin != PIN_DISCONNECTED) {
        pinMode(this->ms1Pin, OUTPUT);
        pinMode(this->ms2Pin, OUTPUT);
        pinMode(this->ms3Pin, OUTPUT);
    }

    if (this->rpm == 0)
        this->rpm = DefaultRpm;

    if (this->acceleration <= 0)
        this->acceleration = DefaultAcceleration;

    if (this->deceleration <= 0)
        this->deceleration = DefaultDeceleration;

    #if _DEBUG
    Serial.println("<- Servo::Setup()");
    #endif

    return this->Enable();
}

// ---------- Private methods ----------

void Servo::CalculateStepPulse()
{
    if (steps_remaining <= 0) {
        return;
    }

    this->steps_remaining--;
    this->step_count++;

    if (this->speedProfile == ServoSpeedProfile::SSP_Linear) {
        switch (getState()) {
            case ServoState::SM_Accelerating:
                this->step_pulse = this->step_pulse - (2 * this->step_pulse + this->rest) / (4 * this->step_count + 1);

                if (this->step_count < this->steps_to_cruise)
                    this->rest = (2 * this->step_pulse + rest) % (4 * this->step_count + 1);
                else
                    this->rest = 0;

                break;

            case ServoState::SM_Decelerating:
                this->step_pulse = this->step_pulse - (2 * this->step_pulse + this->rest) / (-4 * this->steps_remaining + 1);
                this->rest = (2 * this->step_pulse + this->rest) % (-4 * this->steps_remaining + 1);

                break;

            default:
                break; // no speed changes
        }
    }
}

long Servo::CalculateSteps(long degree)
{
    // Calculate steps needed to rotate requested angle, given in degrees
    return degree * this->motor_steps * this->microsteps / 360;
}

void Servo::UpdateMove(long steps)
{
    switch (this->getState()) {
        case ServoState::SM_Accelerating:
            // this also works but will keep the original speed target

        case ServoState::SM_Cruising:
            if (steps >= 0)
                this->steps_remaining += steps;
            else
                this->steps_remaining = max(this->steps_to_brake, this->steps_remaining + steps);

            break;

        case ServoState::SM_Decelerating:
            // would need to start accelerating again -- NOT IMPLEMENTED
            break;

        case ServoState::SM_Sopped:
            this->BeginMove(steps);
            break;
    }
}

long Servo::YieldMove()
{
    #if _DEBUG
    Serial.println("-> Servo::YieldMove()");
    #endif

    if (this->steps_remaining > 0) {
        delayMicros(this->next_action_interval, this->last_action_end);
        /*
        * DIR pin is sampled on rising STEP edge, so it is set first
        */
        digitalWrite(this->directionPin, this->direction);
        digitalWrite(this->stepPin, HIGH);

        unsigned int m = micros();
        long pulse = this->step_pulse; // save value because calcStepPulse() will overwrite it
        this->CalculateStepPulse();
        m = micros() - m;

        // We should pull HIGH for 1-2us (step_high_min)
        if (m < this->step_high_min) { // fast MCPU or CONSTANT_SPEED
            delayMicros(this->step_high_min - m);
            m = this->step_high_min;
        };

        digitalWrite(this->stepPin, LOW);

        // account for calcStepPulse() execution time; sets ceiling for max rpm on slower MCUs
        this->last_action_end = micros();
        this->next_action_interval = (pulse > m) ? pulse - m : 1;
    }
    else {
        // end of move
        this->last_action_end = 0;
        this->next_action_interval = 0;
    }

    #if _DEBUG
    Serial.println("<- Servo::YieldMove()");
    #endif

    return next_action_interval;
}