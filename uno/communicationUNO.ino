IBC ibc;

void setup() {
	//IBC standard constructed
}

void nextibc() {
/* IBC_FRAME_GENERATION_TAG_BEGIN */
/* Generated with Uno_ibcgeneration.py */
/* Code inside IBC BEGIN/END MID RECV/SEND tags will be preserved */

		ibc.handleReqHead();

		if(!ibc.STAT())
		switch(MID)
		{

/* IBC_MESSAGE_BEGIN 252 4 8 */
			case 252:

/*	 Recv exactly 4 bytes in the following								*/
/*	 Also calculate their data hash along the way by					*/
/*		xoring all bytes together once									*/
/*		or use the provided function									*/
/*	 Make the hash public to the IBC by ibc.setDH(Your DATAHASH HERE)	*/
/* IBC_PRESERVE_RECV_BEGIN 252 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
				char buff[4];
				for(int i = 0 , i < 4; i++)
				{
					buff[i] = Serial.read();
				}

				// DO NOT FORGET TO SET THE HASH
				ibc.setDH(ibc.createDH(buff, 4));
/* IBC_PRESERVE_RECV_END 252   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */
				if(ibc.STAT())break;
				ibc.handleReqFoot();
				if(ibc.STAT())break;
				ibc.handleResHead();	
				if(ibc.STAT())break;


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
		}
		if(ibc.STAT())
		{
			//TODO SLEEP FOR A TIME
			while(Serial.available > 0)Serial.read() // empty sent data
			ibc.negativeResponse();
		}
		else
		{
			ibc.handleResFoot();
		}
/* IBC_FRAME_GENERATION_TAG_END */
}
