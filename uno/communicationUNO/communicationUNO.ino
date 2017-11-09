#include <LiquidCrystal.h>

// only for debug --------------------------------------------
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int cursorposition = 0;
//------------------------------------------------------------


void setup() {

  Serial.begin(57600); //opens serial port... sets data rate to 57600 baud
  lcd.begin(16,2);
  
}

void loop() {

  while (Serial.available() > 0 ) {

    char incomingByte = Serial.read(); // read byte

// only for debug --------------------------------------------    
    if(cursorposition <= 14){
      
      lcd.setCursor(cursorposition,0);
      cursorposition += 1;
      
     }else{
      
       cursorposition = 0;
     }
     
    lcd.print(incomingByte);
//------------------------------------------------------------
    
    Serial.write(incomingByte);        // send it back
  }

}
