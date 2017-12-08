void setup() {

  Serial.begin(9600); //opens serial port... sets data rate to 57600 baud

}

char createSH(char SIZE)
{
	return ((SIZE ^ (SIZE >> 2) ^ (SIZE >> 4) ^ (SIZE >> 6)) & 0x03) << 2;
}

char createHH_response(char STAT)
{
	return (STAT ^ (STAT >> 2)) & 0x03;
}

char createHH_request(char MID , char STAT)
{
	return (MID ^ (MID >> 2) ^ (MID >> 4) ^ (MID >> 6) ^ SIZE ^ (SIZE >> 2)) & 0x03;
}

char createDH(char* data, int size, char in)
{
	int i;
	for(i = 0; i < size ; i++)
	{
		in ^= data[i];
	}
	return in;
}

char createDH(char* data, int size)
{
	createDH(data, size, 0);
}

struct serialfield
{
	char IN_MID, IN_STAT, IN_SH, IN_HH, IN_DH;

	char THIS_STAT = 0;
	char CALC_DH;

	char EID;
};

void init_serialfield(serialfield *s)
{
	*s = {0,0,0,0,0,0,0,0};
}

/**
* @brief Errorhandling on Datahashes
*	Checks the equality of datahash and calculated hash and reacts to a mismatch.
*	Call this function when the next byte to receive is the to receive data hash.
*
* @param THIS_STAT	The Address where the IBP-Status of this slave is stored in the lowest 4 bits.
* @param CALC_DH	The datahash which was calculated on this slave.
*
* @return	true = success , false = failure : All internal actions should already be performed by this function any way, so this return serves only convenience. 
*/
bool checkDH(serialfield *sf)
{
	sf->IN_DH = Serial.read();
	if(sf->IN_DH == sf->CALC_DH)	return true;
	
	//set status bit for failed data hash
	sf->THIS_STAT |= 0x01;
	return false;
}

bool checkSH(serialfield *sf)
{
	if(createSH(sf->IN_SIZE) == IN_SH) return true;

	//set status bit for failed size hash
	sf->THIS_STAT |= 0x02;
	return false;
}

bool checkHH_request(serialfield *sf)
{
	if(createHH_request(sf->IN_MID, sf->IN_STAT) == sf->IN_HH)	return true;

	//set status bit for failed headhash
	sf->THIS_STAT |= 0x04;
	return false;
}

void externalError(serialfield *sf, char EID)
{
	sf->EID = EID;
	sf->THIS_STAT |= 0x08;
}

bool negResponse(serialfield *sf)
{
	if(sf->THIS_STAT) return false;

	//construct the negative response here

	send(createStatusByte(sf));
	send(sf->MID);
	send(sf->EID);
	return true;
}

void newrequest(serialfield *sf)
{
		sf->IN_MID = recv(); // read first byte supposed to be id
		sf->IN_STAT = recv(); //read second byte supposed to be status and hashes

		sf->IN_SH = (sf->IN_STAT & 0x0C) >> 2;
		sf->IN_HH = sf->IN_STAT & 0x03;
		sf->IN_STAT = sf->IN_STAT >> 4;
}

char createStatusByte(serialfield *sf, char dynsize)
{
	return (THIS_STAT << 4) & createSH(dynsize) & createHH(STAT);
}

char createStatusByte(serialfield *sf)
{
	return createStatusByte(sf, 0);
}

//TODO THIS_sendmultiple THIS_recvmultiple overall own send functions

void recv(char * c, int length)
{
	if(Serial.available()>=length)
	{
		int i;
		for(i = 0; i < length; i++)
		{
			c[i] = Serial.read();
		}
	}
}

char recv()
{
	char c;
	recv(&c, 1);
	return c;
}

char

void loop() {
	IBC ibc;	
	while (1) 
	{

/* IBC_FRAME_GENERATION_TAG_BEGIN */
/* Generated with Uno_ibcgeneration.py */
/* Code inside IBC BEGIN/END MID RECV/SEND tags will be preserved */

		ibc.handleReqHead();

		switch(MID)
		{

/* IBC_MESSAGE_BEGIN 252 4 8 */
			case 252:

			// Recv exactly 4 bytes in the following
			// Also calculate their data hash along the way by
			//		xoring all bytes together once
			//		or use the provided function
			// Make the hash public to the IBC by ibc.setDH(Your DATAHASH HERE)
/* IBC_PRESERVE_RECV_BEGIN 252 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
				char buff[4];
				for(int i = 0 , i < 4; i++)
				{
					buff[i] = Serial.read();
				}

				// DO NOT FORGET TO SET THE HASH
				ibc.setDH(ibc.createDH(buff, 4));
/* IBC_PRESERVE_RECV_END 252   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */

				ibc.handleReqFoot();
				ibc.handleResHead();
				
			//Send exactly 8 bytes in the following
			// Also calculate their data hash along the way by
			//		xoring all bytes together once
			//		or use the provided function ibc.createDH(..)
			// Make the hash public to the IBC by ibc.setDH(Your DATAHASH HERE)
/* IBC_PRESERVE_SEND_BEGIN 252 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
				

				Serial.write(5);
				Serial.write(6);
				Serial.write(7);
				Serial.write(8);

				Serial.write(5);
				Serial.write(6);
				Serial.write(7);
				Serial.write(8);

				
				// DO NOT FORGET TO SET THE HASH
				ibc.setDH( /*DH*/ );
/* IBC_PRESERVE_SEND_END 252   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */
			break;
/* IBC_MESSAGE_END 252 4 8 */
			case 253:
				char size = Serial.read();

				//if you are dynamic you have to check the sizehash
				checkSH(size);

				char * buff = new char [size];

				int i;
				for(i = 0; i < size ; i++)
				{
					buff[i] = Serial.read();
				}

				//check data hash
				CALC_DH = createDH(buff, size);
				checkDH(&THIS_STAT, CALC_DH);

				//do smth with received data

				//status and hashes
				Serial.write(0);

				//data
				Serial.write(2);
				Serial.write(2);

				char x [2] = {2, 2};
				DH = createDH(x , 2);
			break;

			case 254:
				
			break;
		}
		ibc.handleResFoot();
/* IBC_FRAME_GENERATION_TAG_END */
	}
}
