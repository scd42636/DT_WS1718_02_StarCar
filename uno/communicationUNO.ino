void setup() {

  Serial.begin(9600); //opens serial port... sets data rate to 57600 baud

}

void loop() {

  while (Serial.available() > 0 ) {

    char id = Serial.read(); // read first byte supposed to be id
	char status = Serial.read(); //read second byte supposed to be status and hashes
	
	//TODO check status
	//TODO check hashes

	switch(id)
	{
		case 252:

		char buff[4];
			for(int i = 0 , i < 4; i++)
			{	
				buff[i] = Serial.read();
			}

			//check data hash here
			char hash = buff[0] ^ buff[1] ^ buff[2] ^ buff[3];

			char receivedhash = Serial.read();

			if(hash != receivedhash)
			{
				//TODO errorhandling
			}

			//send answer from here
		
			//construct status and hashes
			Serial.write(0x00);

			//Datagramm
			Serial.write(5);
			Serial.write(6);
			Serial.write(7);
			Serial.write(8);

			Serial.write(5);
			Serial.write(6);
			Serial.write(7);
			Serial.write(8);

			//hash
			Serial.write(0);
		break;	
	}

    Serial.write(incomingByte);        // send it back

  }

}
