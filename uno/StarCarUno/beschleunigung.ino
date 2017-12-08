#include <Wire.h>


#define ADXL345_Address         0x00
#define ADXL345_REG_BW_RATE     0x2C   
#define ADXL345_REG_DATA_FORMAT 0x31
#define ADXL345_REG_POWER_CTL   0x2D
#define ADXL345_REG_DATAX0      0x32    // X-axis data 0
#define ADXL345_REG_DATAY0      0x34    // Y-axis data 0
#define ADXL345_MG2G_MULTIPLIER 0.004

void setup(void) 
{
  Serial.begin(9600);  
  /* Initialise the sensor */
  Wire.begin();
  Wire.beginTransmission(ADXL345_Address);
  Wire.endTransmission();
  
  // Set the accuracy 
  Write(ADXL345_REG_DATA_FORMAT, 0b00);
  // enable measurements
  Write(ADXL345_REG_POWER_CTL, 0x08); 
 }

void loop(void) 
{
  int Xvalue, Yvalue;
  float Xaccel, Yaccel;

 Xvalue = Read(ADXL345_REG_DATAX0);
 Yvalue = Read(ADXL345_REG_DATAY0);

 Xaccel = Xvalue * 9.81;
 Yaccel = Yvalue * 9.81;
  
  
  delay(500);
}
int Read(int address)
{
  int variable;
  Wire.beginTransmission(ADXL345_Address);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(ADXL345_Address, 2);
  variable = Wire.read() | (Wire.read() << 8);
  return(variable);
}
void Write(int address, int data)
{
  Wire.beginTransmission(ADXL345_Address);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}
