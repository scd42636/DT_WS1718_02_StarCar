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
    this->isFirstRun = true;
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
        if (this->isFirstRun) {
            AP_OffRequest offRequest;
            AP_OffResponse offResponse;

            if (this->SendAndReceive(&offRequest, &offResponse)) {
                #if _DEBUG
                Serial.println("AP: OFF");
                #else
                delay(5);
                #endif

                this->state = StarWatchState::WatchState_AccessPoint_Off;

                AP_ResetRequest resetRequest;
                AP_ResetResponse resetResponse;

                if (this->SendAndReceive(&resetRequest, &resetResponse)) {
                    #if _DEBUG
                    Serial.println("AP: Reset");
                    #else
                    delay(5);
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
        else if (this->state == StarWatchState::WatchState_AccessPoint_Off) {
            AP_OnRequest request;
            AP_OnResponse response;

            if (this->SendAndReceive(&request, &response)) {
                #if _DEBUG
                Serial.println("AP: ON");
                #else
                delay(5);
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
        else if (this->state == StarWatchState::WatchState_AccessPoint_On) {
            AP_GetStatusRequest request;
            AP_GetStatusResponse response;

            if (this->SendAndReceive(&request, &response)) {
                #if _DEBUG
                Serial.print("AP: GetStatus = ");
                Serial.println(response.Status);
                #else
                delay(5);
                #endif

                if (response.Status == AccessPointStatusCode::AP_STA_SimplicitLinked)
                    this->state = StarWatchState::WatchState_AccessPoint_Listening;
            }
            else {
                #if _DEBUG
                Serial.print("AP: GetStatus - failed, Result=");
                Serial.println(response.Result);
                #endif
            }
        }
        else if (this->state == StarWatchState::WatchState_AccessPoint_Listening) {
            AP_GetSimplicitiDataRequest request;
            AP_GetSimplicitiDataResponse response;

            if (this->SendAndReceive(&request, &response)) {
                StarWatchButton button = response.Data.Values.Button;

                // Check if response is ACC response.
                if (response.Data.Raw.Value > 255) {
                    float_t x = response.Data.Values.AccelerationX;
                    float_t y = response.Data.Values.AccelerationY;

                    if (x > 127)
                        x = -(255 - x);
                    if (y > 127)
                        y = -(255 - y);

                    if (x != 0 || y != 0) {
                        short_t speed = 0;

                        if (x < 0)
                            speed = (-x / 50.0) * 100;
                        else
                            speed = (-x / 40.0) * 100;

                        if (speed > -10 && speed < 10)
                            speed = 0;
                        else if (speed < -100)
                            speed = -100;
                        else if (speed > 100)
                            speed = 100;

                        car->setSpeed((sbyte_t)((float_t)speed * 0.8));
                        short_t direction = ((y / 45.0) * 1.6) * 100;

                        if (direction > -10 && direction < 10)
                            direction = 0;
                        else if (direction < -100)
                            direction = -100;
                        else if (direction > 100)
                            direction = 100;

                        car->setDirection(direction);

                        #if TEST
                        Serial.print("AP: GetSimplicitiData");
                        Serial.print(", ");
                        Serial.print("X=");
                        Serial.print(x);
                        Serial.print(", ");
                        Serial.print("Y=");
                        Serial.print(y);

                        Serial.print(" -> Speed = ");
                        Serial.print(speed);

                        Serial.print(", Direction = ");
                        Serial.println(direction);
                        #endif
                    }
                }

                if (button == StarWatchButton::WatchButton_TopLeft) {
                    Serial.print(button);
                    Serial.print(", ");

                    StarCarEngineMode engineMode = car->getEngineMode();

                    if (engineMode == StarCarEngineMode::CarEngineMode_Off)
                        car->setEngineMode(StarCarEngineMode::CarEngineMode_On);
                    else
                        car->setEngineMode(StarCarEngineMode::CarEngineMode_Off);

                    delay(250);
                }
            }
            else {
                #if _DEBUG
                Serial.print("AP: GetSimplicitiData - failed, Result=");
                Serial.println(response.Result);
                #endif
            }
        }

        this->isFirstRun = false;
    }
    else {
        this->isFirstRun = true;
    }
}

// ---------- Protected methods ----------

byte_t StarWatch::InitCore()
{
    return StarWatchResult::WatchResult_Success;
}

// ---------- Private methods ----------

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
    byte_t rcode = this->controller->RcvData(length, data);

    if (rcode && rcode != hrNAK)
        return false;

    return true;
}

bool StarWatch::TransmitFrame(byte_t* data, ushort_t length)
{
    byte_t rcode = this->controller->SndData(length, data);

    if (rcode)
        return false;

    return true;
}
