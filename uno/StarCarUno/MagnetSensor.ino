#include <Wire.h>
#include <EEPROM.h>

// configure the compass as reqiured
#define OSR 0b00 // over sampling rate set to 512. 0b01 is 256, 0b10 is 128 and 0b11 is 64
#define RNG 0b00 // Full Scale set to +/- 2 Gauss, 0b01 is +/- 8.
#define ODR 0b00 // output data rate set to 10Hz, 0b01 is 50Hz, 0b10 is 100Hz, 0b11 is 200Hz
#define MODE 0b01 // continuous measurement mode, 0b00 is standby
#define CR2 0b00000000 // control register 2: disable soft reset and pointer rollover, interrupt pin not enabled
#define RESET 0b00000001


// Reference the I2C Library
#define HMC5883L_Address 0x0D
#define ControlRegister1 0x09
#define ControlRegister2 0x0A
#define DataRegisterBegin 0x00 // from 00H to 05H

int error = 0;
float Scale;
int c;

float ConvertToPositive (float convert){
  if (convert > 180){
    convert = 180-convert;
    }
  if (convert < 0){
    convert = 0-convert;
    c = 1; //1 entspricht negativer Zahl
    }
   else{
    c = 0;
    }
    return convert;
  
}

// Out setup routine, here we will configure the microcontroller and compass.
void setup()
{
// Initialize the serial port.
Serial.begin(9600);

Wire.begin(); // Start the I2C interface.
// Check that a device responds at the compass address - don't continue if it doesn't -
do {
delay(100);
Wire.beginTransmission(HMC5883L_Address);
error = Wire.endTransmission();
} while (error);

// configure the control registers using static settings above
// compass autoranges, but starts in the mode given
Write(ControlRegister1, MODE);
Write(ControlRegister2, CR2);
// Write(SetResetRegister, RESET);
switch (MODE & 0b00010000){
case 0b00010000:
Scale = (float) 8 / (float) 32768;
break;
default:
Scale = (float) 2 / (float) 32768;
break;
}
}

// Our main program loop.
void loop()
{
int XAxis, YAxis;
float Xscale, Yscale;

// Retrive the raw values from the compass (not scaled).
Wire.beginTransmission(HMC5883L_Address);
Wire.write(DataRegisterBegin);
Wire.endTransmission();

Wire.beginTransmission(HMC5883L_Address);
Wire.requestFrom(HMC5883L_Address, 9);
if(9<=Wire.available())
{
XAxis = Wire.read()<<8; //MSB
XAxis |= Wire.read(); //LSB
YAxis = Wire.read()<<8;
YAxis |= Wire.read();
}

// Retrived the scaled values from the compass (scaled to the configured scale).
Xscale = XAxis * Scale;
Yscale = YAxis * Scale;

// Calculate heading when the magnetometer is level, then correct for signs of axis.
// relative to a magnetometer mounted with 'Y' axis pointing in the direction to be measured
// and with 'X' pointing to the right from the perspective of the operator facing in the +Y direction
// heading (degrees): 0 = +X, 90 = +Y, 180 = -X, 270 = -Y
float heading = atan2(Yscale, Xscale);

// Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
float declinationAngle = 0.05515; // compass reads magnetic north for those who remember how to read maps and adjust for declination
heading += declinationAngle;

// Correct for when signs are reversed.
if(heading < 0)
heading += 2*PI;

// Check for wrap due to addition of declination.
if(heading > 2*PI)
heading -= 2*PI;

// Convert radians to degrees for readability.
float headingDegrees = heading * 180/M_PI;

headingDegrees = ConvertToPositive (headingDegrees);

// Output the data via the serial port.
Output(c, headingDegrees);

// 1. delay the application by 100ms to allow the loop to run at 10Hz (default bandwidth for the HMC5883L)
delay(1000);
}

void Write(int address, int data)
{
Wire.beginTransmission(HMC5883L_Address);
Wire.write(address);
Wire.write(data);
Wire.endTransmission();
}


// Output the data down the serial port.

void Output(int c, float headingDegrees)
{
//Write to EEPROM
EEPROM.write(0, c);
EEPROM.write(1, headingDegrees);
int x = EEPROM.read (0);
int y = EEPROM.read (1);
Serial.print(x, DEC);
Serial.println();
Serial.print(y, DEC);
Serial.println();

}

// Cacluate bearing from heading.
// relative to a magnetometer mounted with 'Y' axis pointing in the direction to be measured
// and with 'X' pointing to the right from the perspective of the operator facing in the +Y direction
// bearing 0 = Y pointing North, 90 = Y pointing E, 180 = Y pointing S, 270 = Y pointing W
float bearingDegrees(float headingDegrees) {

float bearing = headingDegrees - 90;
if (bearing < 0)
{
bearing += 360;
}
return bearing;

}
