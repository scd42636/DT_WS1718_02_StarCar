//--------------------------------------------------------------------------------------------------
// <copyright file="StarMagnetometer.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Annkathrin Bauer</author>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "StarMagnetometer.h"
#include <Wire.h>

// Configure the compass as reqiured
#define OSR 0b00 // Over sampling rate set to 512. 0b01 is 256, 0b10 is 128 and 0b11 is 64
#define RNG 0b00 // Full Scale set to +/- 2 Gauss, 0b01 is +/- 8.
#define ODR 0b00 // output data rate set to 10Hz, 0b01 is 50Hz, 0b10 is 100Hz, 0b11 is 200Hz
#define MODE 0b01 // Continuous measurement mode, 0b00 is standby
#define CR2 0b00000000 // Control register 2: disable soft reset and pointer rollover, interrupt pin not enabled
#define RESET 0b00000001

// Reference the I2C Library
#define HMC5883L_Address 0x0D
#define ControlRegister1 0x09
#define ControlRegister2 0x0A
#define DataRegisterBegin 0x01 // from 00H to 05H


// ---------- Public constructors ----------

StarMagnetometer::StarMagnetometer()
{
    this->isConnected = false;
    this->lastRequestTime = 0;
    this->scaling = 0.0;
}

// ---------- Public properties ----------

const char* StarMagnetometer::getName()
{
    return "Magnetometer";
}

// ---------- Public methods ----------

void StarMagnetometer::Task(StarCar* car)
{
    #if _DEBUG
    Serial.println("--> StarMagnetometer::Task()");
    #endif

    ulong_t currentRequestTime = millis();

    // Delay the next request by 100ms to allow the loop to run at 10Hz
    // (default bandwidth for the HMC5883L).
    ulong_t duration = currentRequestTime - this->lastRequestTime;

    if (this->isConnected
        && (this->lastRequestTime == 0 || duration >= 100)
        && car->IsRequested(StarCarSensorRequest::CarSensorRequest_Magnet)) {
        int_t xAxis, yAxis;
        float_t xScale, yScale;

        // Retrive the raw values from the compass (not scaled).
        Wire.beginTransmission(HMC5883L_Address);
        Wire.write(DataRegisterBegin);
        Wire.endTransmission();

        Wire.beginTransmission(HMC5883L_Address);
        Wire.requestFrom(HMC5883L_Address, 9);

        if (9 <= Wire.available()) {
            xAxis = Wire.read() << 8;   // MSB
            xAxis |= Wire.read();       // LSB
            yAxis = Wire.read() << 8;   // MSB
            yAxis |= Wire.read();       // LSB
        }

        // Retrieve the scaled values from the compass (scaled to the configured scale).
        xScale = xAxis * this->scaling;
        yScale = yAxis * this->scaling;

        // Calculate heading when the magnetometer is level, then correct for signs of axis.
        // Relative to a magnetometer mounted with 'Y' axis pointing in the direction to be
        // measured and with 'X' pointing to the right from the perspective of the operator
        // facing in the +Y direction heading (degrees): 0 = +X, 90 = +Y, 180 = -X, 270 = -Y
        float_t heading = atan2(yScale, xScale);

        ////// Once you have your heading, you must then add your 'Declination Angle', which is
        ////// the 'Error' of the magnetic field in your location.
        ////// Compass reads magnetic north for those who remember how to read maps and adjust
        ////// for declination.
        ////float declinationAngle = heading + 0.05515;

        // Correct for when signs are reversed.
        if (heading < 0)
            heading += 2 * PI;

        // Check for wrap due to addition of declination.
        if (heading > 2 * PI)
            heading -= 2 * PI;

        // Convert radians to degrees for readability.
        float_t degrees = heading * 180 / PI;

        if (degrees > 180)
            degrees = 180 - degrees;

        car->setOrientation(degrees);

        #if TEST
        Serial.print(this->getName());
        Serial.print(": ");

        Serial.print("Orientation = ");
        Serial.println(degrees);
        #endif

        this->lastRequestTime = currentRequestTime;
    }
}

// ---------- Protected methods ----------

byte_t StarMagnetometer::InitCore()
{
    // Start the I2C interface.
    Wire.begin();

    delay(100);

    // Check that a device responds at the compass address.
    Wire.beginTransmission(HMC5883L_Address);

    if (!Wire.endTransmission()) {
        this->isConnected = true;

        // Configure the control registers using static settings above compass autoranges, but
        // starts in the mode given
        Write(ControlRegister1, MODE);
        Write(ControlRegister2, CR2);

        // Write(SetResetRegister, RESET);

        switch (MODE & 0b00010000) {
            case 0b00010000:
                this->scaling = (float_t)8 / (float_t)32768;
                break;
            default:
                this->scaling = (float_t)2 / (float_t)32768;
                break;
        }

        return StarMagnetometerResult::MagnetometerResult_Success;
    }

    return StarMagnetometerResult::MagnetometerResult_NotConnected;
}

// ---------- Private methods ----------

void StarMagnetometer::Write(int_t address, int_t data)
{
    Wire.beginTransmission(HMC5883L_Address);

    Wire.write(address);
    Wire.write(data);

    Wire.endTransmission();
}
