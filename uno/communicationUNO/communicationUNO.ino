#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {

  Serial.begin(57600); //opens serial port... sets data rate to 57600 baud
  lcd.begin(16, 2);
  lcd.print("hello, world!");
}

void loop() {

  while (Serial.available() > 0 ) {

    char incomingByte = Serial.read(); // read byte
    
    Serial.write(incomingByte);        // send it back
  }

}
