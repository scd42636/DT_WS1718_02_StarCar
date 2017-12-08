
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

void handleReqDyn(){
	m_INSIZE_DYN = recv();
	
	checkinSH();
}

void IBC::handleReqFoot(){
	checkinDH();
}

void IBC::handleRes(){

}

void IBC::extError(byte EID){
	m_EID = EID;
	m_STAT |= STAT_ERROR_EXT;
}

void IBC::handleErrors(){
	if(!m_STAT) return;
}
