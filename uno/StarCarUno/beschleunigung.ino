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

void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event; 
  accel.getEvent(&event);
 
  /* Display the results (acceleration is measured in m/s^2) */
  //Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  //Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");

  //Write to EEPROM 
  
float Xaccel = event.acceleration.x;
Serial.print(Xaccel);
Serial.println();
Xaccel = ConvertToPositive(Xaccel);

EEPROM.put(9, Xaccel);
float test = EEPROM.get(9, Xaccel);
Serial.print(test);
Serial.println();
delay (1000);
Serial.print("STOPP");

Serial.println();

Serial.print(Xaccel);
Serial.println();
Serial.print("Ende");
Serial.println();
//fehler nach Ende
EEPROM.write(0, c);
EEPROM.write(1, Xaccel);
int uw = EEPROM.read (0);
int xt = EEPROM.read (1);
Serial.print(uw, DEC);
Serial.println();
Serial.print("Positiv oder negativ");
Serial.println();
Serial.print(xt, DEC);
Serial.println();
Serial.print("Beschleunigung in X");
Serial.println();

float Yaccel = event.acceleration.y;
Serial.print(Yaccel);
Serial.println();
Yaccel = ConvertToPositive(Yaccel);

EEPROM.put(10, Yaccel);
float test2 = EEPROM.get(10, Yaccel);
Serial.print(test2);
Serial.println();
delay (1000);
Serial.print("STOPP");

Serial.println();

Serial.print(Yaccel);
Serial.println();
Serial.print("Ende");
Serial.println();
//fehler nach Ende
EEPROM.write(0, c);
EEPROM.write(1, Yaccel);
int u = EEPROM.read (0);
int xs = EEPROM.read (1);
Serial.print(u, DEC);
Serial.println();
Serial.print("Positiv oder negativ");
Serial.println();
Serial.print(xs, DEC);
Serial.println();
Serial.print("Beschleunigung in Y");
Serial.println();

  delay(500);
}
