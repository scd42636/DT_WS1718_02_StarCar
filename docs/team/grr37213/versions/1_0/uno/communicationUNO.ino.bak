void setup() {

  Serial.begin(9600); //opens serial port... sets data rate to 57600 baud

}

void loop() {

  while (Serial.available() > 0 ) {

    char incomingByte = Serial.read(); // read byte
    Serial.write(incomingByte);        // send it back

  }

}
