//--------------------------------------------------------------------------------------------------
// <copyright file="StarWatch.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

// Implemented using the protocol documented on:
// - http://www.instructables.com/id/Control-an-Arduino-With-a-Wristwatch-TI-eZ430-Chr/

#pragma once
#include "../StarCar.h"


enum StarWatchState
{
    WatchState_AccessPoint_Error = -1,
    WatchState_AccessPoint_Off = 0,
    WatchState_AccessPoint_On = 1,
    WatchState_AccessPoint_Listening = 2,
};

enum StarWatchButton : byte_t
{
    WatchButton_Unkown = 0,
    WatchButton_None = 1,
    WatchButton_TopLeft = 17,
    WatchButton_TopRight = 49,
    WatchButton_BottomLeft = 33,
};

enum StarWatchDevice : byte_t
{
    WatchDevice_Unkown = 0,
    WatchDevice_DirectionWheel = 10,
    WatchDevice_SpeedWheel = 11
};

enum StarWatchResult
{
    WatchResult_Success = 0,
    WatchResult_Failed = -1,
};

enum AccessPointCommandCode : byte_t
{
    AP_CMD_GetStatus = 0x00,
    AP_CMD_Reset = 0x01,
    AP_CMD_StartSimpliciti = 0x07,
    AP_CMD_GetSimplicitiData = 0x08,
    AP_CMD_StopSimpliciti = 0x09,
};

enum AccessPointStatusCode : byte_t
{
    AP_STA_Idle = 0x00,
    AP_STA_SimplicitiStopped = 0x01,
    AP_STA_SimplicitiTryingToLink = 0x02,
    AP_STA_SimplicitiLinked = 0x03,
    AP_STA_Error = 0x05,
    AP_STA_NoError = 0x06,
    AP_STA_NotConnected = 0x07,
    AP_STA_SimplicitiLinkTimeout = 0x08,
};

#define AP_PacketStartMarker    0xFF
#define AP_PacketOverheadBytes  0x03

typedef struct AP_Request_t
{
    byte_t Start = AP_PacketStartMarker;
    AccessPointCommandCode Command = (AccessPointCommandCode)0;
    byte_t Length = AP_PacketOverheadBytes;
} AP_Request;

typedef struct AP_Response_t
{
    byte_t Start = AP_PacketStartMarker;
    AccessPointStatusCode Result = (AccessPointStatusCode)0;
    byte_t Length = AP_PacketOverheadBytes;
} AP_Response;



typedef struct AP_OnRequest_t : AP_Request_t
{
    AP_OnRequest_t()
    {
        Command = AccessPointCommandCode::AP_CMD_StartSimpliciti;
    }
} AP_OnRequest;
typedef struct AP_OnResponse_t : AP_Response_t
{
} AP_OnResponse;


typedef struct AP_OffRequest_t : AP_Request_t
{
    AP_OffRequest_t()
    {
        Command = AccessPointCommandCode::AP_CMD_StopSimpliciti;
    }
} AP_OffRequest;
typedef struct AP_OffResponse_t : AP_Response_t
{
} AP_OffResponse;


typedef struct AP_GetStatusRequest_t : AP_Request_t
{
    AccessPointStatusCode Status = (AccessPointStatusCode)0;

    AP_GetStatusRequest_t()
    {
        Command = AccessPointCommandCode::AP_CMD_GetStatus;
        Length += 1; // Status
    }
} AP_GetStatusRequest;
typedef struct AP_GetStatusResponse_t : AP_Response_t
{
    AccessPointStatusCode Status = (AccessPointStatusCode)0;

    AP_GetStatusResponse_t()
    {
        Length += 1; // Status
    }
} AP_GetStatusResponse;


typedef struct AP_ResetRequest_t : AP_Request_t
{
    byte_t Reserved = 0;

    AP_ResetRequest_t()
    {
        Command = AccessPointCommandCode::AP_CMD_Reset;
        Length += 1; // Reserved
    }
} AP_ResetRequest;
typedef struct AP_ResetResponse_t : AP_Response_t
{
    byte_t Reserved = 0;

    AP_ResetResponse_t()
    {
        Length += 1; // Reserved
    }
} AP_ResetResponse;


typedef union AP_SimplicitiRawData_t
{
    dword_t Value;
    byte_t Values[5];
} AP_SimplicitiRawData;


typedef struct AP_SimplicitiData_t
{
    StarWatchButton Button;
    byte_t AccelerationX;
    byte_t AccelerationY;
    byte_t AccelerationZ;
    StarWatchDevice Device;
} AP_SimplicitiData;
typedef struct AP_GetSimplicitiDataRequest_t : AP_Request_t
{
    union RequestData
    {
        AP_SimplicitiRawData Raw;
        AP_SimplicitiData Values;
    } Data;

    AP_GetSimplicitiDataRequest_t()
    {
        Command = AccessPointCommandCode::AP_CMD_GetSimplicitiData;
        Length += sizeof(RequestData);

        memset(&this->Data, 0, sizeof(RequestData));
    }
} AP_GetSimplicitiDataRequest;
typedef struct AP_GetSimplicitiDataResponse_t : AP_Response_t
{
    union ResponseData
    {
        AP_SimplicitiRawData Raw;
        AP_SimplicitiData Values;
    } Data;

    AP_GetSimplicitiDataResponse_t()
    {
        Length += sizeof(ResponseData);
        memset(&this->Data, 0, sizeof(ResponseData));
    }
} AP_GetSimplicitiDataResponse;


class StarWatch : public StarCarModule
{
    // ---------- Private fields ----------
private:
    UsbController* controller;
    StarWatchState state;
    bool isAccessPointInitialized;

    // ---------- Public constructors ----------
public:
    StarWatch(UsbController* controller);

    // ---------- Public properties ----------
public:
    virtual const char* getName();
    StarWatchState getState();

    // ---------- Public methods ----------
public:
    virtual void Task(StarCar* car);

    // ---------- Protected methods ----------
protected:
    virtual byte_t InitCore();

    // ---------- Private methods ----------
private:
    void ActivateAccessPoint();
    void InitializeAccessPoint();

    void UpdateState();

    bool RequestControlValue(
        StarWatchDevice* device,
        StarWatchButton* button,
        sbyte_t* controlValue,
        sbyte_t* positionValue);

    bool SendAndReceive(AP_Request* request, AP_Response* response);

    bool ReceiveFrame(byte_t* frame, uint_t* length);
    bool TransmitFrame(byte_t* frame, uint_t length);
};
