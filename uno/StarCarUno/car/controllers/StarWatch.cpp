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

            if (this->state == StarWatchState::WatchState_AccessPoint_Listening) {
                sbyte_t oldDirectionValue = car->getDirection();
                sbyte_t oldSpeedValue = car->getSpeed();

                sbyte_t newDirectionValue = oldDirectionValue;
                sbyte_t newSpeedValue = oldSpeedValue;

                if (this->RequestControlValue(&newDirectionValue, &newSpeedValue)) {
                    car->setDirection(newDirectionValue);
                    car->setSpeed(newSpeedValue);

                    if (car->getEngineMode() == StarCarEngineMode::CarEngineMode_Off) {
                        car->setEngineMode(StarCarEngineMode::CarEngineMode_On);
                        delay(250);
                    }

                    #if TEST
                    Serial.print("AP: GetSimplicitiData: ");

                    Serial.print("Speed = ");
                    Serial.print(newSpeedValue);

                    Serial.print(", Direction = ");
                    Serial.println(newDirectionValue);
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
    sbyte_t* directionValue,
    sbyte_t* speedValue)
{
    bool success = false;

    AP_GetSimplicitiDataRequest request;
    AP_GetSimplicitiDataResponse response;

    if (this->SendAndReceive(&request, &response)) {
        success = true;

        #if _DEBUG
        Serial.print("AP: GetSimplicitiData = ");
        Serial.println(response.Status);
        #endif

        if (response.Data.Values.DataReceived) {
            if (response.Data.Values.DirectionValue != 250) {
                float_t direction = response.Data.Values.DirectionValue;
                *directionValue = this->CalculateControlValue(direction);
            }

            if (response.Data.Values.SpeedValue != 250) {
                float_t speed = response.Data.Values.SpeedValue;
                *speedValue = this->CalculateControlValue(speed);
            }

            success = true;
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

sbyte_t StarWatch::CalculateControlValue(float_t value)
{
    // Align value around zero and determine -/+ range.
    if (value > 127)
        value = -(255 - value);

    // Calculate percentage portion of the value relative to its possible range.
    if (value < 0)
        value = (-value / 50.0) * 100;
    else
        value = (-value / 40.0) * 100;

    // Skip values lower than 5%.
    if (value > -5 && value < 5)
        value = 0;

    // Align values lower than -100% to exact -100%.
    if (value < -100)
        value = -100;
    // Align values greater than 100% to exact 100%.
    else if (value > 100)
        value = 100;

    if (value < 0) {
        // Align value to maximum possible motion range.
        // The motion range -60% to 100% (without alignment).
        value = (value / 60.0) * 100;

        // Align values lower than -100% to exact -100%.
        if (value < -100)
            value = -100;
    }

    return (sbyte_t)value;
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
            || status == AccessPointStatusCode::AP_STA_SimplicitiLinked) {
            this->state = StarWatchState::WatchState_AccessPoint_Listening;
        }
        else if (status == AccessPointStatusCode::AP_STA_SimplicitiStopped) {
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

        uint_t responseLength = response->Length;

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

bool StarWatch::ReceiveFrame(byte_t* data, uint_t* length)
{
    byte_t rcode = this->controller->RcvData(length, (uint8_t*)data);

    if (rcode && rcode != hrNAK)
        return false;

    return true;
}

bool StarWatch::TransmitFrame(byte_t* data, uint_t length)
{
    byte_t rcode = this->controller->SndData(length, (uint8_t*)data);

    if (rcode)
        return false;

    return true;
}
