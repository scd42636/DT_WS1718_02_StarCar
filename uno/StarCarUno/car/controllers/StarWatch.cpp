//--------------------------------------------------------------------------------------------------
// <copyright file="StarWatch.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "StarWatch.h"


// ---------- Public constructors ----------

StarWatch::StarWatch(UsbController* controller)
{
    this->controller = controller;

    this->state = StarWatchState::WatchState_AccessPoint_Off;
    this->isAccessPointInitialized = true;
}

// ---------- Public properties ----------

const char* StarWatch::getName()
{
    return "Watch";
}

StarWatchState StarWatch::getState()
{
    return this->state;
}

// ---------- Public methods ----------

void StarWatch::Task(StarCar* car)
{
    #if _DEBUG
    Serial.println("--> StarWatch::Task()");
    #endif

    if (!this->controller->isReady())
        return;

    if (car->getMode() == StarCarMode::CarMode_Watch) {
        if (!this->isAccessPointInitialized) {
            this->InitializeAccessPoint();
            this->isAccessPointInitialized = true;
        }
        else if (this->state == StarWatchState::WatchState_AccessPoint_Off) {
            this->ActivateAccessPoint();
        }
        else if (this->state >= StarWatchState::WatchState_AccessPoint_On) {
            this->UpdateState();

            if (this->state == StarWatchState::WatchState_AccessPoint_On) {
                car->setEngineMode(StarCarEngineMode::CarEngineMode_Off);
            }
            else if (this->state == StarWatchState::WatchState_AccessPoint_Listening) {
                StarWatchDevice device = StarWatchDevice::WatchDevice_Unkown;
                StarWatchButton button = StarWatchButton::WatchButton_Unkown;

                sbyte_t controlValue = 0;
                sbyte_t positionValue = 0;

                if (this->RequestControlValue(&device, &button, &controlValue, &positionValue)) {
                    sbyte_t oldValue = 0;
                    sbyte_t newValue = controlValue;

                    if (device == StarWatchDevice::WatchDevice_SpeedWheel) {
                        oldValue = car->getSpeed();

                        // Check if watch is in a valid horizontal "command area".
                        if (positionValue < -5 || positionValue > 5)
                            newValue = 0;

                        newValue = (float_t)newValue * 0.8;
                        car->setSpeed(newValue);
                    }
                    else if (device == StarWatchDevice::WatchDevice_DirectionWheel) {
                        oldValue = car->getDirection();

                        // Check if watch is in a valid horizontal "command area".
                        if (positionValue < -15 || positionValue > 0)
                            newValue = 0;

                        car->setDirection(newValue);
                    }

                    if (button == StarWatchButton::WatchButton_TopLeft) {
                        car->SwitchEngineMode();
                        delay(250);
                    }

                    #if !TEST
                    if (oldValue != newValue) {
                        Serial.print("AP: GetSimplicitiData");

                        Serial.print(": Device = ");
                        Serial.print(device);

                        Serial.print(" -> Speed = ");
                        Serial.print(car->getSpeed());

                        Serial.print(", Direction = ");
                        Serial.println(car->getDirection());
                    }
                    #endif
                }
            }
        }
    }
}

// ---------- Protected methods ----------

byte_t StarWatch::InitCore()
{
    return StarWatchResult::WatchResult_Success;
}

// ---------- Private methods ----------

void StarWatch::ActivateAccessPoint()
{
    AP_OnRequest request;
    AP_OnResponse response;

    if (this->SendAndReceive(&request, &response)) {
        #if _DEBUG
        Serial.println("AP: ON");
        #endif

        this->state = StarWatchState::WatchState_AccessPoint_On;
    }
    else {
        #if _DEBUG
        Serial.print("AP: ON - failed, Result=");
        Serial.println(response.Result);
        #endif
    }
}

void StarWatch::InitializeAccessPoint()
{
    AP_OffRequest offRequest;
    AP_OffResponse offResponse;

    if (this->SendAndReceive(&offRequest, &offResponse)) {
        #if _DEBUG
        Serial.println("AP: OFF");
        #endif

        this->state = StarWatchState::WatchState_AccessPoint_Off;
        delay(5);

        AP_ResetRequest resetRequest;
        AP_ResetResponse resetResponse;

        if (this->SendAndReceive(&resetRequest, &resetResponse)) {
            #if _DEBUG
            Serial.println("AP: Reset");
            #endif
        }
        else {
            #if _DEBUG
            Serial.print("AP: Reset - failed, Result=");
            Serial.println(resetResponse.Result);
            #endif
        }
    }
    else {
        #if _DEBUG
        Serial.print("AP: OFF - failed, Result=");
        Serial.println(offResponse.Result);
        #endif
    }
}

bool StarWatch::RequestControlValue(
    StarWatchDevice* device,
    StarWatchButton* button,
    sbyte_t* controlValue,
    sbyte_t* positionValue)
{
    bool success = false;

    *device = StarWatchDevice::WatchDevice_Unkown;
    *button = StarWatchButton::WatchButton_None;
    *controlValue = 0;

    AP_GetSimplicitiDataRequest request;
    AP_GetSimplicitiDataResponse response;

    if (this->SendAndReceive(&request, &response)) {
        success = true;

        #if _DEBUG
        Serial.print("AP: GetSimplicitiData = ");
        Serial.println(response.Status);
        #endif

        *device = response.Data.Values.Device;
        *button = response.Data.Values.Button;

        // Check if response is ACC response.
        if (response.Data.Raw.Value > 255) {
            float_t x = response.Data.Values.AccelerationX;
            float_t y = response.Data.Values.AccelerationY;

            // Align value around zero and determine -/+ range.
            if (x > 127)
                x = -(255 - x);

            // Align value around zero and determine -/+ range.
            if (y > 127)
                y = -(255 - y);

            // Calculate percentage portion of the value relative to its possible range.
            if (x < 0)
                x = (-x / 50.0) * 100;
            else
                x = (-x / 40.0) * 100;

            // Skip values lower than 5%.
            if (x > -5 && x < 5)
                x = 0;

            // Align values lower than -100% to exact -100%.
            if (x < -100)
                x = -100;
            // Align values greater than 100% to exact 100%.
            else if (x > 100)
                x = 100;

            if (x < 0) {
                // Align value to maximum possible motion range.
                // The motion range -60% to 100% (without alignment).
                x = (x / 60.0) * 100;

                // Align values lower than -100% to exact -100%.
                if (x < -100)
                    x = -100;
            }

            *controlValue = x;
            *positionValue = y;
        }
    }
    else {
        #if _DEBUG
        Serial.print("AP: GetSimplicitiData - failed, Result=");
        Serial.println(response.Result);
        #endif
    }

    return success;
}

void StarWatch::UpdateState()
{
    AP_GetStatusRequest request;
    AP_GetStatusResponse response;

    if (this->SendAndReceive(&request, &response)) {
        #if _DEBUG
        Serial.print("AP: GetStatus = ");
        Serial.println(response.Status);
        #endif

        AccessPointStatusCode status = response.Status;

        if (status == AccessPointStatusCode::AP_STA_Idle
            || status == AccessPointStatusCode::AP_STA_SimplicitLinked) {
            this->state = StarWatchState::WatchState_AccessPoint_Listening;
        }
        else if (status == AccessPointStatusCode::AP_STA_SimplicitStopped) {
            this->state = StarWatchState::WatchState_AccessPoint_Off;
        }
        else if ((byte_t)status == 255) {
            // The access point will send this status in case it is ready but did not yet
            // set up its internal status appropriate.
            this->state = StarWatchState::WatchState_AccessPoint_Listening;
        }
        else {
            ////Serial.println(status);
            this->state = StarWatchState::WatchState_AccessPoint_On;
        }
    }
    else {
        #if _DEBUG
        Serial.print("AP: GetStatus - failed, Result=");
        Serial.println(response.Result);
        #endif
    }
}

bool StarWatch::SendAndReceive(AP_Request* request, AP_Response* response)
{
    bool success = false;

    byte_t* requestData = (byte_t*)malloc(request->Length);
    memcpy(requestData, request, request->Length);

    if (this->TransmitFrame(requestData, request->Length)) {
        delay(1); // Wait a while before trying to receive the response.

        byte_t* responseData = (byte_t*)malloc(response->Length);
        memset(responseData, 0, response->Length);

        ushort_t responseLength = response->Length;

        if (this->ReceiveFrame(responseData, &responseLength)) {
            response->Length = responseLength;

            memcpy(response, responseData, response->Length);
            success = response->Result == AccessPointStatusCode::AP_STA_NoError;
        }

        free(responseData);
    }

    free(requestData);
    return success;
}

bool StarWatch::ReceiveFrame(byte_t* data, ushort_t* length)
{
    byte_t rcode = this->controller->RcvData(length, (uint8_t*)data);

    if (rcode && rcode != hrNAK)
        return false;

    return true;
}

bool StarWatch::TransmitFrame(byte_t* data, ushort_t length)
{
    byte_t rcode = this->controller->SndData(length, (uint8_t*)data);

    if (rcode)
        return false;

    return true;
}
