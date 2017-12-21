#ifndef IBC_H
#     define IBC_H

#define IBC_BAUD 115200
#include <EEPROM.h>
class IBC
{
    byte DH;

  public:
    IBC();
    void next();

    void send(byte* d, int length);
    void send(byte d);
    void recv(byte*d, int length);
    byte recv();

    void setDH(byte dh);

    byte createDH(byte* b, int lenght);
    byte createDH(byte* b, int lenght, byte in);
};
#endif /* IBC_H */

IBC::IBC()
{
  Serial.begin(IBC_BAUD);
  DH = 0;
}

void IBC::send(byte* d, int length)
{
  for (int i = 0; i < length; i++)
  {
    Serial.write(d[i]);
  }
}

void IBC::send(byte d)
{
  this->send(&d, 1);
}

void IBC::recv(byte* d, int length)
{
  while (Serial.available() < length);
  for (int i = 0; i < length; i++)
  {
    d[i] = Serial.read();
  }
}

byte IBC::recv()
{
  byte ret;
  recv(&ret, 1);
  return ret;
}


void IBC::setDH(byte dh)
{
  DH = dh;
}

byte IBC::createDH(byte* b, int length)
{
  return createDH(b, length, 0);
}

byte IBC::createDH(byte* b, int length, byte in)
{
  for (int i = 0; i < length; i++)
  {
    in ^= b[i];
  }
  return in;
}

void IBC::next() {
/* IBC_FRAME_GENERATION_TAG_BEGIN */                                                     
/* Generated with Uno_ibcgeneration.py */
/* Code inside IBC BEGIN/END MID RECV/SEND tags will be preserved */

  char sstat = 0x00;

  char mid = recv();
  char midstat = recv();

  send(sstat);

  char mstat = recv();

  switch ((unsigned byte)mid)
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

        byte buffr0[0];
        recv(buffr0, 0);

        //DONT FORGET TO HASH
        setDH(createDH(buffr0, 0));

/* IBC_PRESERVE_RECV_END 0 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

  char datahash = recv();
  send(sstat);

/*Send exactly 0 bytes in the following                  */
/*Also calculate their data hash along the way by                   */
/*  xoring all bytes together once                                  */
/*  or use the provided function createDH(..)                   */
/* Make the hash public to the IBC by setDH(Your DATAHASH HERE) */
/* IBC_PRESERVE_SEND_BEGIN 0 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/

        for (int i = 0 ; i < 0; i++) {
          send(0);
        }

        //DONT FORGET TO HASH
        setDH(0);

/* IBC_PRESERVE_SEND_END 0 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
        }
        break;
/* IBC_MESSAGE_END 0 0 0 */
/* IBC_MESSAGE_BEGIN 180 0 2 */
        case 180:
        {
           

/*   Recv exactly 0 bytes in the following                              */
/*   Also calculate their data hash along the way by                    */
/*      xoring all bytes together once                                  */
/*      or use the provided function                                    */
/*   Make the hash public to the IBC by setDH(Your DATAHASH HERE)   */
/* IBC_PRESERVE_RECV_BEGIN 180 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
			
			//DONT FORGET TO HASH
			setDH(0);


      
/* IBC_PRESERVE_RECV_END 180 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

  char datahash = recv();
  send(sstat);

/*Send exactly 2 bytes in the following                  */
/*Also calculate their data hash along the way by                   */
/*  xoring all bytes together once                                  */
/*  or use the provided function createDH(..)                   */
/* Make the hash public to the IBC by setDH(Your DATAHASH HERE) */
/* IBC_PRESERVE_SEND_BEGIN 180 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
  int SonicFront =  EEPROM.read(0);
  byte * bpSF = (byte*)&SonicFront;
  
			send(bpSF, sizeof(int));
			
			//DONT FORGET TO HASH
			setDH(createDH(bpSF, 2));
/* IBC_PRESERVE_SEND_END 180 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
        }
        break;
/* IBC_MESSAGE_END 180 0 2 */
/* IBC_MESSAGE_BEGIN 181 0 2 */
        case 181:
        {
           

/*   Recv exactly 0 bytes in the following                              */
/*   Also calculate their data hash along the way by                    */
/*      xoring all bytes together once                                  */
/*      or use the provided function                                    */
/*   Make the hash public to the IBC by setDH(Your DATAHASH HERE)   */
/* IBC_PRESERVE_RECV_BEGIN 181 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
			
			byte buffr181[0];
			recv(buffr181,0);
			
			//DONT FORGET TO HASH
			setDH(createDH(buffr181,0));
			
/* IBC_PRESERVE_RECV_END 181 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

  char datahash = recv();
  send(sstat);

/*Send exactly 2 bytes in the following                  */
/*Also calculate their data hash along the way by                   */
/*  xoring all bytes together once                                  */
/*  or use the provided function createDH(..)                   */
/* Make the hash public to the IBC by setDH(Your DATAHASH HERE) */
/* IBC_PRESERVE_SEND_BEGIN 181 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
			int SonicBack =   EEPROM.read(1);
      byte * bpSB =(byte*) &SonicBack;
      
			for(int i = 0 ; i<2;i++) {send(bpSB[i]);}
			//DONT FORGET TO HASH
			setDH(createDH(bpSB, 2));
			
/* IBC_PRESERVE_SEND_END 181 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
        }
        break;
/* IBC_MESSAGE_END 181 0 2 */
/* IBC_MESSAGE_BEGIN 182 0 3 */
        case 182:
        {
           

/*   Recv exactly 0 bytes in the following                              */
/*   Also calculate their data hash along the way by                    */
/*      xoring all bytes together once                                  */
/*      or use the provided function                                    */
/*   Make the hash public to the IBC by setDH(Your DATAHASH HERE)   */
/* IBC_PRESERVE_RECV_BEGIN 182 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
			
			//DONT FORGET TO HASH
			setDH(0);

     
/* IBC_PRESERVE_RECV_END 182 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

  char datahash = recv();
  send(sstat);

/*Send exactly 3 bytes in the following                  */
/*Also calculate their data hash along the way by                   */
/*  xoring all bytes together once                                  */
/*  or use the provided function createDH(..)                   */
/* Make the hash public to the IBC by setDH(Your DATAHASH HERE) */
/* IBC_PRESERVE_SEND_BEGIN 182 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
		 byte parityMag = EEPROM.read (3);
     int Mag = EEPROM.read (4);

     send(parityMag);
     
     byte * bpM = (byte*)&Mag;
     send(bpM, 2);
			
			//DONT FORGET TO HASH
			setDH( createDH(&parityMag,1,createDH(bpM, 2)));
			
/* IBC_PRESERVE_SEND_END 182 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
        }
        break;
/* IBC_MESSAGE_END 182 0 3 */
/* IBC_MESSAGE_BEGIN 183 0 6 */
        case 183:
        {
           

/*   Recv exactly 0 bytes in the following                              */
/*   Also calculate their data hash along the way by                    */
/*      xoring all bytes together once                                  */
/*      or use the provided function                                    */
/*   Make the hash public to the IBC by setDH(Your DATAHASH HERE)   */
/* IBC_PRESERVE_RECV_BEGIN 183 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
			
			//DONT FORGET TO HASH
		  setDH(0);
    
/* IBC_PRESERVE_RECV_END 183 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

  char datahash = recv();
  send(sstat);

/*Send exactly 6 bytes in the following                  */
/*Also calculate their data hash along the way by                   */
/*  xoring all bytes together once                                  */
/*  or use the provided function createDH(..)                   */
/* Make the hash public to the IBC by setDH(Your DATAHASH HERE) */
/* IBC_PRESERVE_SEND_BEGIN 183 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
			byte parityXacc = EEPROM.read(5);
      int Xaccel = EEPROM.read(6);
      byte parityYacc = EEPROM.read(7);
      int Yaccel = EEPROM.read(8);

     send(parityXacc);
     byte * bpXacc = (byte*)&Xaccel;
     send(bpXacc, 2);
     send(parityYacc);
     byte * bpYacc =(byte*) &Yaccel;
     send(bpYacc, 2);
			
			//DONT FORGET TO HASH
			setDH( createDH(&parityXacc,1,createDH(bpXacc, 2, createDH(&parityYacc, 1, createDH(bpYacc, 2)))));
			
/* IBC_PRESERVE_SEND_END 183 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
        }
        break;
/* IBC_MESSAGE_END 183 0 6 */
/* IBC_MESSAGE_BEGIN 254 4 8 */
        case 254:
        {
           

/*   Recv exactly 4 bytes in the following                              */
/*   Also calculate their data hash along the way by                    */
/*      xoring all bytes together once                                  */
/*      or use the provided function                                    */
/*   Make the hash public to the IBC by setDH(Your DATAHASH HERE)   */
/* IBC_PRESERVE_RECV_BEGIN 254 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/

        byte buffr254[4];
        recv(buffr254, 4);

        //DONT FORGET TO HASH
        setDH(createDH(buffr254, 4));

/* IBC_PRESERVE_RECV_END 254 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

  char datahash = recv();
  send(sstat);

/*Send exactly 8 bytes in the following                  */
/*Also calculate their data hash along the way by                   */
/*  xoring all bytes together once                                  */
/*  or use the provided function createDH(..)                   */
/* Make the hash public to the IBC by setDH(Your DATAHASH HERE) */
/* IBC_PRESERVE_SEND_BEGIN 254 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/

        byte buff254[8] = {1,2,3,4,5,6,7,8};
        send (buff254, 8);

        //DONT FORGET TO HASH
        setDH(8);

/* IBC_PRESERVE_SEND_END 254 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
        }
        break;
/* IBC_MESSAGE_END 254 4 8 */
/* IBC_MESSAGE_BEGIN 253 16 4 */
        case 253:
        {
           

/*   Recv exactly 16 bytes in the following                              */
/*   Also calculate their data hash along the way by                    */
/*      xoring all bytes together once                                  */
/*      or use the provided function                                    */
/*   Make the hash public to the IBC by setDH(Your DATAHASH HERE)   */
/* IBC_PRESERVE_RECV_BEGIN 253 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
			
			byte buffr253[16];
			recv(buffr253,16);
			
			//DONT FORGET TO HASH
			setDH(createDH(buffr253,16));
			
/* IBC_PRESERVE_RECV_END 253 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

  char datahash = recv();
  send(sstat);

/*Send exactly 4 bytes in the following                  */
/*Also calculate their data hash along the way by                   */
/*  xoring all bytes together once                                  */
/*  or use the provided function createDH(..)                   */
/* Make the hash public to the IBC by setDH(Your DATAHASH HERE) */
/* IBC_PRESERVE_SEND_BEGIN 253 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/

      char message [4] = "me2";
			send(((byte*)message), 4);
			
			//DONT FORGET TO HASH
			setDH(createDH(((byte*)message), 4));
			
/* IBC_PRESERVE_SEND_END 253 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
        }
        break;
/* IBC_MESSAGE_END 253 16 4 */
  }
  send(DH);
/* IBC_FRAME_GENERATION_TAG_END */

      //  byte sstat = 0x00;
      //
      //  byte mid = recv();
      //  byte midstat = recv();
      //
      //  send(sstat);
      //
      //  byte mstat = recv();
      //
      //  switch ((unsigned byte)mid)
      //  {
      //    case 254:
      //      /* RECV */
      //      byte buffr254 [4];
      //      recv(buffr254, 4);
      //
      //
      //      byte datahash = recv();
      //      send(sstat);
      //
      //      /* SEND */
      //
      //      byte buffs254 [8] = {'d', 'e', 'r', 'e', 5, 6, 7, 8};
      //      send(buffs254, 8);
      //
      //      //hash
      //      send(8);
      //      break;
      //  }

  }

  IBC* ibc;

  void setup()
  {
    ibc = new IBC;
  }

  void loop()
  {
    ibc->next();
  }

