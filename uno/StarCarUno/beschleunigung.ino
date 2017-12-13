#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

int c;

float ConvertToPositive (float convert){
  if (convert < 0){
    convert = 0-convert;
    c = 1; //1 entspricht negativer Zahl
    }
   else{
    c = 0;
    }
    return convert; 
}

int ShiftToInteger(float wert){
  wert = wert * 100;
  
  int shiftwert = (int)wert;
 
 // float shiftwert = wert << 2; 
  //int neuerWert = shiftwert <<2;
  Serial.print(shiftwert);
  Serial.println();
  Serial.print("Shiftwert");
  return shiftwert;
  }

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Accelerometer Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }

  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_2_G);
  // displaySetRange(ADXL345_RANGE_8_G);
  // displaySetRange(ADXL345_RANGE_4_G);
  // displaySetRange(ADXL345_RANGE_2_G);
  
  Serial.println("");
}

void getSensorStart(){
  /* Get a new sensor event */ 
  sensors_event_t event; 
  accel.getEvent(&event);
 
  /* Display the results (acceleration is measured in m/s^2) */
  //Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  //Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");

  //Write to EEPROM 
  
float Xaccel = event.acceleration.x;
Xaccel = ConvertToPositive(Xaccel);
int newXaccel = ShiftToInteger (Xaccel);´
EEPROM.put(9, newXaccel);
delay (1000);

float Yaccel = event.acceleration.y;
Yaccel = ConvertToPositive(Yaccel);
int newYaccel = ShiftToInteger (Yaccel);
EEPROM.put(10, newYaccel);

  delay(1000);
  }

void loop(void) 
{

  getSensorStart();
  
}
