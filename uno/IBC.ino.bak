#include "IBC.h"

IBC::IBC()
{
	Serial.begin(IBC_BAUD);

	m_INMID = 0;
	m_INSTAT = 0;
	m_INSIZE_DYN = 0;
	m_INDH = 0;
	m_STAT = 0;
	m_DH = 0;
	m_EID = 0;
}

byte IBC::inMID(){
	return m_INMID;
}

byte IBC::inStatbyte(){
	return m_INSTAT;
}

byte IBC::inSTAT(){
	return m_INSTAT >> 4;
}

byte IBC::inSIZE_DYN()
{
	return m_INSIZE_DYN;
}

byte IBC::inSH(){
	return m_INSTAT >> 2 & 0x03;
}

byte IBC::inHH(){
	m_INSTAT & 0x03;
}

byte IBC::inDH(){
	return m_INDH;
}

byte IBC::STAT(){
	return m_STAT;
}

byte IBC::HH_request(){
	return	(	m_INMID ^ 
				(m_INMID >> 2) ^ 
				(m_INMID >> 4) ^ 
				(m_INMID >> 6) ^ 
				(m_INSTAT >> 4) ^ 
				(m_INSTAT >> 6)		)	& 0x03;
}

byte IBC::HH_response(){
	return (m_STAT ^ (m_STAT >> 2)) & 0x03;
}

byte IBC::SH(byte size){
	return	(	size ^
				(size >> 2) ^
				(size >> 4) ^
				(size >> 6)		) & 0x03;
}

byte IBC::createDH(byte *data, int size, byte in){
	for(int i = 0; i<size; i++)
	{
		in ^= data[i];
	}
	return in;
}

byte IBC::createDH(byte* data, int size){
	return createDH(data, size, 0);
}

byte IBC::Statbyte(byte dynsize){
	return (STAT << 4) & (SH(dynsize) << 2) & HH_response();
}

byte IBC::Statbyte(){
	return Statbyte(0);
}

bool IBC::checkinHH(){
	return inHH() == HH_request();
}

bool IBC::checkinSH(){
	return inSH() == SH(m_INSIZE_DYN);
}

bool IBC::checkinDH(byte dh){
	return inDH() == m_CALCDH;
}

void IBC::setinSIZE_DYN(byte size){
	inSIZE_DYN = size;
}

void IBC::setDH(byte dh){
	m_DH = dh;
}

byte IBC::DH(){
	return m_DH;
}

void IBC::send(byte *b, int size){
	for(int i=0; i<size; i++)
		Serial.write(*b);
}

void IBC::send(byte b){
	Serial.write(b);
}

void IBC::recv(byte *data, int size){
	int i = 0;
	while(i < size)
	{
		if(Serial.available() > 0)
		{
			data[i] = Serial.read();
			i++;
		}
		else
		{
			//TODO IDLE TIME
		}
	}
}

byte IBC::recv(){
	while(!(Serial.available()>0)){
		//TODO IDLE TIME
	}
	return Serial.read();
}

void IBC::handleReqHead(){
	m_INMID = recv();
	m_INSTAT = recv();

	checkinHH();
}

void IBC::handleReqDyn(){
	m_INSIZE_DYN = recv();
	
	checkinSH();
}

void IBC::handleReqFoot(){
	checkinDH();
}

void IBC::handleResHead(){
	send(Statbyte());
}

void IBC::handleResFoot(){
	send(DH());
}

void IBC::error(byte EID){
	m_EID = EID;
	m_STAT |= STAT_ERROR_EXT;
}

void IBC::negativeResponse(){
	send(Statbyte());
	send(m_MID);
	send(m_EID);
}

void IBC::next(){

/* IBC_FRAME_GENERATION_TAG_BEGIN */                                                     
/* Generated with Uno_ibcgeneration.py */
/* Code inside IBC BEGIN/END MID RECV/SEND tags will be preserved */

        handleReqHead();
     
        if(!STAT())
        switch((unsigned char)inMID())
        {


/* IBC_MESSAGE_BEGIN 0 0 0 */
        case 0:
        {
           

/*   Recv exactly 0 bytes in the following                              */
/*   Also calculate their data hash along the way by                    */
/*      xoring all bytes together once                                  */
/*      or use the provided function                                    */
/*   Make the hash public to the IBC by setDH(Your DATAHASH HERE)   */
/* IBC_PRESERVE_RECV_BEGIN 0 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
			
			char buffr0[0];
			recv(buffr0,0);
			
			//DONT FORGET TO HASH
			setDH(createDH(buffr0,0));
			
/* IBC_PRESERVE_RECV_END 0 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

            if(STAT())break;
            handleReqFoot();
            if(STAT())break;
            handleResHead();
            if(STAT())break;

/*Send exactly 0 bytes in the following                  */
/*If you have a dynamic size you have to send this size first!      */
/*Also calculate their data hash along the way by                   */
/*  xoring all bytes together once                                  */
/*  or use the provided function createDH(..)                   */
/* Make the hash public to the IBC by setDH(Your DATAHASH HERE) */
/* IBC_PRESERVE_SEND_BEGIN 0 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
			
			for(int i = 0 ; i<0;i++) {send(0);}
			
			//DONT FORGET TO HASH
			setDH(0);
			
/* IBC_PRESERVE_SEND_END 0 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
        }
        break;
/* IBC_MESSAGE_END 0 0 0 */
/* IBC_MESSAGE_BEGIN 252 4 8 */
        case 252:
        {
           

/*   Recv exactly 4 bytes in the following                              */
/*   Also calculate their data hash along the way by                    */
/*      xoring all bytes together once                                  */
/*      or use the provided function                                    */
/*   Make the hash public to the IBC by setDH(Your DATAHASH HERE)   */
/* IBC_PRESERVE_RECV_BEGIN 252 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
			
			char buffr252[4];
			recv(buffr252,4);
			
			//DONT FORGET TO HASH
			setDH(createDH(buffr252,4));
			
/* IBC_PRESERVE_RECV_END 252 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

            if(STAT())break;
            handleReqFoot();
            if(STAT())break;
            handleResHead();
            if(STAT())break;

/*Send exactly 8 bytes in the following                  */
/*If you have a dynamic size you have to send this size first!      */
/*Also calculate their data hash along the way by                   */
/*  xoring all bytes together once                                  */
/*  or use the provided function createDH(..)                   */
/* Make the hash public to the IBC by setDH(Your DATAHASH HERE) */
/* IBC_PRESERVE_SEND_BEGIN 252 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
			
			for(int i = 0 ; i<8;i++) {send(0);}
			
			//DONT FORGET TO HASH
			setDH(0);
			
/* IBC_PRESERVE_SEND_END 252 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
        }
        break;
/* IBC_MESSAGE_END 252 4 8 */
/* IBC_MESSAGE_BEGIN 253 255 2 */
        case 253:
        {
           
		handleReqDyn();

/*   Recv exactly inSIZE_DYN() bytes in the following                              */
/*   Also calculate their data hash along the way by                    */
/*      xoring all bytes together once                                  */
/*      or use the provided function                                    */
/*   Make the hash public to the IBC by setDH(Your DATAHASH HERE)   */
/* IBC_PRESERVE_RECV_BEGIN 253 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
			
			char *buffr253 = new char [inSIZE_DYN()];
			recv(buffr253,inSIZE_DYN());
			
			//DONT FORGET TO HASH
			setDH(createDH(buffr253,inSIZE_DYN()));
			delete[] buffr253;//you can delete the buffer in this recv preservation or in the send preservation.. dont forget it 
			
/* IBC_PRESERVE_RECV_END 253 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

            if(STAT())break;
            handleReqFoot();
            if(STAT())break;
            handleResHead();
            if(STAT())break;

/*Send exactly 2 bytes in the following                  */
/*If you have a dynamic size you have to send this size first!      */
/*Also calculate their data hash along the way by                   */
/*  xoring all bytes together once                                  */
/*  or use the provided function createDH(..)                   */
/* Make the hash public to the IBC by setDH(Your DATAHASH HERE) */
/* IBC_PRESERVE_SEND_BEGIN 253 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
			
			for(int i = 0 ; i<2;i++) {send(0);}
			
			//DONT FORGET TO HASH
			setDH(0);
			
/* IBC_PRESERVE_SEND_END 253 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
        }
        break;
/* IBC_MESSAGE_END 253 255 2 */
/* IBC_MESSAGE_BEGIN 254 2 255 */
        case 254:
        {
           

/*   Recv exactly 2 bytes in the following                              */
/*   Also calculate their data hash along the way by                    */
/*      xoring all bytes together once                                  */
/*      or use the provided function                                    */
/*   Make the hash public to the IBC by setDH(Your DATAHASH HERE)   */
/* IBC_PRESERVE_RECV_BEGIN 254 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
			
			char buffr254[2];
			recv(buffr254,2);
			
			//DONT FORGET TO HASH
			setDH(createDH(buffr254,2));
			
/* IBC_PRESERVE_RECV_END 254 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

            if(STAT())break;
            handleReqFoot();
            if(STAT())break;
            handleResHead();
            if(STAT())break;

/*Send exactly your ?DYNAMIC_SIZE? amount of  bytes in the following                  */
/*If you have a dynamic size you have to send this size first!      */
/*Also calculate their data hash along the way by                   */
/*  xoring all bytes together once                                  */
/*  or use the provided function createDH(..)                   */
/* Make the hash public to the IBC by setDH(Your DATAHASH HERE) */
/* IBC_PRESERVE_SEND_BEGIN 254 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
			
			send(?DYNAMIC_SIZE?);
			for(int i = 0 ; i< ?DYNAMIC_SIZE? ;i++) {send(0);}
			
			//DONT FORGET TO HASH
			setDH(0);
			
/* IBC_PRESERVE_SEND_END 254 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
        }
        break;
/* IBC_MESSAGE_END 254 2 255 */
        default : 
            m_STAT = 0x04;
        break;
        }
        if(STAT())
        {
            delay(1000);
            while(Serial.available() > 0)Serial.read(); // empty sent data
            negativeResponse();
            m_STAT = 0;
        }
        else
        {
            handleResFoot();
        }
/* IBC_FRAME_GENERATION_TAG_END */
}
