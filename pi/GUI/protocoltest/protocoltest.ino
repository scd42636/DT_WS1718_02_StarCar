// Copyright Victor Hurdugaci (http://victorhurdugaci.com). All rights reserved.
// Licensed under the Apache License, Version 2.0. See LICENSE in the project root for license information.

// Remove the following line if you are not using an LCD to display data
//#define LCD

#include <SoftwareSerial.h>

#include <ArduinoSerialProtocol.h>

enum StarCarMode
{
  CarMode_None = 0,
  CarMode_Controller = 1,
  CarMode_Watch = 2
};

enum StarCarSensorRequest
{
  CarSensorRequest_None = 0,
  CarSensorRequest_Sonic = 1,
  CarSensorRequest_Magnet = 2,
  CarSensorRequest_Accelerator = 4
};

struct StarBoardExchangeData_t
{
  uint8_t Mode;
  uint8_t Request;

  uint16_t DistanceFront;
  uint16_t DistanceBack;

  uint8_t  DirectionParity;
  uint16_t DirectionValue;

  uint8_t  AccelerationXParity;
  uint16_t AccelerationXValue;

  uint8_t AccelerationYParity;
  uint16_t AccelerationYValue;

} 
StarBoardExchangeData;

struct payload{

  uint8_t firstvalue = 50;
  uint8_t secondvalue = 60;
} 
testData;

/*
  struct SamplePayload {
 int8_t A;
 uint8_t B;
 int16_t C;
 uint16_t D;
 } payload;
 */
//ArduinoSerialProtocol protocol(&Serial, (uint8_t*)&StarBoardExchangeData, sizeof(StarBoardExchangeData));
//ArduinoSerialProtocol protocol(&Serial, (uint8_t*)&payload, sizeof(payload));
uint8_t receiveState;
bool low = false;

void setup()
{
  Serial.begin(115200);

  StarBoardExchangeData.Mode = 0;
  StarBoardExchangeData.Request = 0;
  StarBoardExchangeData.DistanceFront = 250;
  StarBoardExchangeData.DistanceBack = 160;
  StarBoardExchangeData.DirectionParity = 1;
  StarBoardExchangeData.DirectionValue = 365;
  StarBoardExchangeData.AccelerationXParity = 1;
  StarBoardExchangeData.AccelerationXValue = 2000;
  StarBoardExchangeData.AccelerationYParity = 1;
  StarBoardExchangeData. AccelerationYValue = 3500;

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH );
  /*
     payload.A = 0;
   payload.B = 0;
   payload.C = 0;
   payload.D = 0;
   }
   */

  Serial.println(sizeof(StarBoardExchangeData));
  Serial.println(sizeof(uint8_t));
  Serial.println(sizeof(uint16_t));
}
void loop()
{

  for(int i = 0; i < 15){
    if(Serial.availeble()){
    
      StarBoardExchangeData[i++] = Serial.read();
    }
  }
  
  if (Serial.available() > 0) {

    int incomingByte = Serial.read();

    if(incomingByte == 7){

      digitalWrite(LED_BUILTIN, LOW);

      StarBoardExchangeData.Mode += 1;
      StarBoardExchangeData.Request += 1;
      StarBoardExchangeData.DistanceFront += 1;
      StarBoardExchangeData.DistanceBack += 1;
      StarBoardExchangeData.DirectionParity = 1;
      StarBoardExchangeData.DirectionValue += 1;
      StarBoardExchangeData.AccelerationXParity = 1;
      StarBoardExchangeData.AccelerationXValue += 1;
      StarBoardExchangeData.AccelerationYParity = 1;
      StarBoardExchangeData. AccelerationYValue += 1;

      Serial.write((const uint8_t *)&StarBoardExchangeData, sizeof(StarBoardExchangeData));
    }
  }
  /*
  receiveState = protocol.receive();
   
   if (receiveState == ProtocolState::SUCCESS)
   {
   
   if (StarBoardExchangeData.Request == 1) {
   
   //Serial.println("Empfangen");    
   
   //Serial.println(StarBoardExchangeData.Mode);
   
   if (low) {
   digitalWrite(LED_BUILTIN, LOW);
   low = false;
   } else {
   digitalWrite(LED_BUILTIN, HIGH);
   low = true;
   }
   
  /*if (StarBoardExchangeData.DistanceFront == 255) {
   
   StarBoardExchangeData.DistanceFront = 0;
   } else {
   ++StarBoardExchangeData.DistanceFront;
   }
   
   StarBoardExchangeData.Mode = 0;
   StarBoardExchangeData.Request = 0;
   
   protocol.send();
   
   }
   
   }
   */
  delay(1000);
}



