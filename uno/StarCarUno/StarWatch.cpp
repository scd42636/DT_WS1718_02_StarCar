//--------------------------------------------------------------------------------------------------
// <copyright file="StarWatch.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "StarWatch.h"


enum AccessPointRequest
{
    // Byte 0: Start marker (0xFF)
    AP_REQ_Start = 0,

    // Byte 1: Command code
    AP_REQ_Command = 1,

    // Byte 2: Packet size (including overhead)
    AP_REQ_Size = 2,

    // Byte 3: Data 3..size
    AP_REQ_StartOfData = 3
};

enum AccessPointResponse
{
    // Byte 0: Start marker (0xFF)
    AP_RES_Start = 0,

    // Byte 1: Status code
    AP_RES_State = 1,
};

enum AccessPointCommandCode
{
    AP_CMD_GetStatus = 0x00,
    AP_CMD_GetProductId = 0x20,
    AP_CMD_StartBSL = 0x21,
    AP_CMD_StartSimplicti = 0x07,
    AP_CMD_GetSimplicitData = 0x08,
    AP_CMD_StopSimplicit = 0x09
};

enum AccessPointStatusCode
{
    AP_STA_Idle = 0x00,
    AP_STA_SimplicitStopped = 0x01,
    AP_STA_SimplicitTryingToLink = 0x02,
    AP_STA_SimplicitLinked = 0x03,
    AP_STA_BlueRobingStopped = 0x04,
    AP_STA_BlueRobinTransmitting = 0x05,
    AP_STA_Error = 0x05,
    AP_STA_NoError = 0x06,
    AP_STA_NotConnected = 0x07,
    AP_STA_SimplicitLinkTimeout = 0x08,
    AP_STA_WbslTryingToLink = 0x09,
    AP_STA_WbslLinked = 0x0A,
    AP_STA_WbslError = 0x0B,
    AP_STA_WbslStopped = 0x0C,
    AP_STA_WbslLinkTimeout = 0x0D,
};

enum AccessPointSync
{
    AP_SYNC_Start = 0x30,
    AP_SYNC_SendCommand = 0x31,
    AP_SYNC_GetBufferStatus = 0x32,
    AP_SYNC_ReadBuffer = 0x33
};

#define AP_PacketStartMarker 0xFF

uint8_t AP_ON_COMMAND[] = {
    AP_PacketStartMarker,
    AP_CMD_StartSimplicti, 0x03 };

uint8_t AP_OFF_COMMAND[] = {
    AP_PacketStartMarker,
    AP_CMD_StopSimplicit, 0x03 };

uint8_t AP_ACC_DATA_REQUEST[] = {
    AP_PacketStartMarker,
    AP_CMD_GetSimplicitData, 0x07, 0x00, 0x00, 0x00, 0x00 };


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

    if (car->getMode() == StarCarMode::CM_Watch) {
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
