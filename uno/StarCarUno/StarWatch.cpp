//--------------------------------------------------------------------------------------------------
// <copyright file="StarWatch.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "StarWatch.h"

// Packet bytes
//
// Byte 0	        Start marker (0xFF)
#define FRAME_BYTE_START          (0)
// Byte 1	        Command code
#define FRAME_BYTE_CMD            (1)
// Byte 2	        Packet size (including overhead)
#define FRAME_BYTE_SIZE           (2)
// Byte 3..packet_size  Data
#define FRAME_BYTE_FIRST_DATA     (3)

uint8_t AP_ON_COMMAND[] = { 0xFF, 0x07, 0x03 };
uint8_t AP_OFF_COMMAND[] = { 0xFF, 0x09, 0x03 };

uint8_t AP_ACC_DATA_REQUEST[] = { 0xFF, 0x08, 0x07, 0x00, 0x00, 0x00, 0x00 };


// ---------- Public constructors ----------

StarWatch::StarWatch(UsbController* controller)
{
    this->controller = controller;

    this->state = StarWatchState::WS_AP_Off;
    this->isFirstRun = true;
}

// ---------- Public properties ----------

StarWatchState StarWatch::getState()
{
    return this->state;
}

// ---------- Public methods ----------

StarWatchResult StarWatch::Init()
{
    return StarWatchResult::WR_Success;
}

void StarWatch::Task(StarCar* car)
{
    #if _DEBUG
    Serial.println("--> StarWatch::Task()");
    #endif

    if (!this->controller->isReady())
        return;

    if (this->isFirstRun) {
        if (this->TransmitFrame(AP_OFF_COMMAND, sizeof(AP_OFF_COMMAND))) {
            Serial.println("AP: Deactivated");
            this->state = StarWatchState::WS_AP_Off;
        }
    }
    else if (this->state == StarWatchState::WS_AP_Off) {
        if (this->TransmitFrame(AP_ON_COMMAND, sizeof(AP_ON_COMMAND))) {
            Serial.println("AP: Activated");
            this->state = StarWatchState::WS_AP_Idle;
        }
    }
    else if (this->state == StarWatchState::WS_AP_Idle) {
        if (this->TransmitFrame(AP_ACC_DATA_REQUEST, sizeof(AP_ACC_DATA_REQUEST))) {
            this->state = StarWatchState::WS_AP_Listening;
        }
    }
    else if (this->state == StarWatchState::WS_AP_Listening) {
        uint8_t frame[64];
        uint16_t length = 64;

        if (this->ReceiveFrame(frame, &length)) {
            if (length == 7) {
                byte type = (byte)frame[3];

                if (type == 0x01) {
                    int x = (int8_t)frame[4];
                    int y = (int8_t)frame[5];
                    int z = (int8_t)frame[6];

                    Serial.print("X = ");
                    Serial.print(x);

                    Serial.print(", Y = ");
                    Serial.print(y);

                    Serial.print(", Z = ");
                    Serial.println(z);

                    delay(2000);
                }
            }
        }
    }

    this->isFirstRun = false;
}

// ---------- Private methods ----------

bool StarWatch::ReceiveFrame(uint8_t* frame, uint16_t* length)
{
    uint8_t rcode = this->controller->RcvData(length, frame);

    if (rcode && rcode != hrNAK)
        return false;

    return true;
}

bool StarWatch::TransmitFrame(uint8_t* frame, uint16_t length)
{
    uint8_t rcode = this->controller->SndData(length, frame);

    if (rcode)
        return false;

    return true;
}
