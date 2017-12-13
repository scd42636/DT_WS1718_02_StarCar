#include <EEPROM.h>
const int pingFront = 7;
const int pingBack = 8;


long SecondsToCentimeters (long microseconds) {
  //calculation in the documentation
  return microseconds / 29 / 2;
} 

void GettingSensors(){
  //establishe variables for duration of the ping
  long duration, cm;
  
  //Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingFront,OUTPUT);
  digitalWrite(pingFront,LOW);
  delayMicroseconds(2);
  digitalWrite(pingFront,HIGH);
  delayMicroseconds(5);
  digitalWrite(pingFront,LOW);
  
   pinMode(pingBack,OUTPUT);
  digitalWrite(pingBack,LOW);
  delayMicroseconds(2);
  digitalWrite(pingBack,HIGH);
  delayMicroseconds(5);
  digitalWrite(pingBack,LOW);
  

  //Same pin is ued to read and receive
  pinMode(pingFront,INPUT);
  duration = pulseIn(pingFront,HIGH);
  
  pinMode(pingBack,INPUT);
  duration = pulseIn(pingBack,HIGH);
  
  //convert the time into a distance
  cm = SecondsToCentimeters(duration);

  EEPROM.write(0, pingFront);
  EEPROM.write(1, pingBack);
  delay(100);
  }

void setup() {
  //initialize serial communication
  Serial.begin (9600);
}

void loop(){

  GettingSensors();

}
