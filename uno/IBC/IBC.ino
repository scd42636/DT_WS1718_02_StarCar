#ifndef IBC_H
#     define IBC_H

#define IBC_BAUD 57600

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
        for (int i = 0 ; i < 8; i++) {
          send(buff254[i]);
        }

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

