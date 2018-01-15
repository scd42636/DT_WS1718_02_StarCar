#include "IbcDriver.h"

IbcDriver driver(0);

void setup() {
      Serial.begin(115200);
}

void loop() {
  int value = millis() / 1000;
  
  EEPROM.write(EEPROM_SONIC_BACK_VALUE, value);
  EEPROM.write(EEPROM_SONIC_FRONT_VALUE, value);

  //Serial.println(value);
  
  driver.next((void*)NULL);
  delay(1000);
}
