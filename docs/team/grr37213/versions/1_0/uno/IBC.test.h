#ifndef IBC_TEST_H
#     define IBC_TEST_H

#define IBC_BAUD 9600

#define STAT_ERROR_EXT 0x08
#define STAT_ERROR_HH 0x04
#define STAT_ERROR_SH 0x02
#define STAT_ERROR_DH 0x01

class IBC
{
public:
	byte m_INMID;		//incoming id 
	byte m_INSTAT;		//status byte goes here
	byte m_INSIZE_DYN;	//incoming dynamic size if set
	byte m_INDH;		//incoming data hash

	byte m_STAT;	//stat of this slave using low 4 bits (0x0F)
	byte m_DH;		//DATAHASH own calculated one

	byte m_EID;
public:
	IBC();

	byte inMID();
	byte inStatbyte();
	byte inSTAT();
	byte inSIZE_DYN();
	byte inSH();
	byte inHH();
	byte inDH();

	byte STAT();

	//hashes
	byte HH_request();
	byte HH_response();
	byte SH(byte size);
	byte createDH(byte *data, int size, byte in);
	byte createDH(byte *data, int size);

	//create full status byte for response purpose
	byte Statbyte(byte dynsize);
	byte Statbyte();

	//check incoming hashes
	bool checkinHH();
	bool checkinSH();
	bool checkinDH(byte dh);

	void setDH(byte DH);
	byte DH();

	void send(byte b);
	void send(byte* b, int size);

	byte recv();
	void recv(byte *b, int size);

	void handleReqHead();
	void handleReqDyn();
	void handleReqFoot();

	void handleResHead();
	void handleResFoot();

	void error(byte EID);
	void negativeResponse();

	void next();
};
#endif /* IBC_TEST_H */
