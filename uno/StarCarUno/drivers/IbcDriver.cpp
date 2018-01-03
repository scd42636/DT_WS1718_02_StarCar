//--------------------------------------------------------------------------------------------------
// <copyright file="IbcDriver.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Robert Graf</author>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "IbcDriver.h"


IbcDriver::IbcDriver(int baud)
{
    Serial.begin(baud);
    DH = 0;
}

void IbcDriver::send(byte* d, int length)
{
    for (int i = 0; i < length; i++) {
        Serial.write(d[i]);
    }
}

void IbcDriver::send(byte d)
{
    this->send(&d, 1);
}

int IbcDriver::recv(byte* d, int length)
{
    if (Serial.available()) {
        while (Serial.available() < length);

        for (int i = 0; i < length; i++) {
            d[i] = Serial.read();
        }

        return length;
    }

    return -1;
}

byte IbcDriver::recv()
{
    byte ret = -1;
    recv(&ret, 1);
    return ret;
}


void IbcDriver::setDH(byte dh)
{
    DH = dh;
}

byte IbcDriver::createDH(byte* b, int length)
{
    return createDH(b, length, 0);
}

byte IbcDriver::createDH(byte* b, int length, byte in)
{
    for (int i = 0; i < length; i++) {
        in ^= b[i];
    }
    return in;
}

void IbcDriver::next(StarCar* car)
{
    /* IBC_FRAME_GENERATION_TAG_BEGIN */
    /* Generated with Uno_ibcgeneration.py */
    /* Code inside IBC BEGIN/END MID RECV/SEND tags will be preserved */

    char sstat = 0x00;

    char mid = recv();

    if (mid == -1)
        return;

    char midstat = recv();

    send(sstat);

    char mstat = recv();

    switch ((unsigned char)mid) {


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

            for (int i = 0; i < 0; i++) {
                send(0);
            }

            //DONT FORGET TO HASH
            setDH(0);

            /* IBC_PRESERVE_SEND_END 0 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
        }
        break;
        /* IBC_MESSAGE_END 0 0 0 */
        /* IBC_MESSAGE_BEGIN 100 0 0 */
        case 100:
        {
            /*   Recv exactly 0 bytes in the following                              */
            /*   Also calculate their data hash along the way by                    */
            /*      xoring all bytes together once                                  */
            /*      or use the provided function                                    */
            /*   Make the hash public to the IBC by setDH(Your DATAHASH HERE)   */
            /* IBC_PRESERVE_RECV_BEGIN 100 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
            car->setMode(StarCarMode::CarMode_Controller);

            //DONT FORGET TO HASH
            setDH(0);

            /* IBC_PRESERVE_RECV_END 100 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

            char datahash = recv();
            send(sstat);

            /*Send exactly 0 bytes in the following                  */
            /*Also calculate their data hash along the way by                   */
            /*  xoring all bytes together once                                  */
            /*  or use the provided function createDH(..)                   */
            /* Make the hash public to the IBC by setDH(Your DATAHASH HERE) */
            /* IBC_PRESERVE_SEND_BEGIN 100 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/

            //DONT FORGET TO HASH
            setDH(0);

            /* IBC_PRESERVE_SEND_END 100 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
        }
        break;
        /* IBC_MESSAGE_END 100 0 0 */
        /* IBC_MESSAGE_BEGIN 101 0 0 */
        case 101:
        {
            /*   Recv exactly 0 bytes in the following                              */
            /*   Also calculate their data hash along the way by                    */
            /*      xoring all bytes together once                                  */
            /*      or use the provided function                                    */
            /*   Make the hash public to the IBC by setDH(Your DATAHASH HERE)   */
            /* IBC_PRESERVE_RECV_BEGIN 101 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
            car->setMode(StarCarMode::CarMode_Watch);

            byte buffr101[0];
            recv(buffr101, 0);

            //DONT FORGET TO HASH
            setDH(createDH(buffr101, 0));

            /* IBC_PRESERVE_RECV_END 101 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

            char datahash = recv();
            send(sstat);

            /*Send exactly 0 bytes in the following                  */
            /*Also calculate their data hash along the way by                   */
            /*  xoring all bytes together once                                  */
            /*  or use the provided function createDH(..)                   */
            /* Make the hash public to the IBC by setDH(Your DATAHASH HERE) */
            /* IBC_PRESERVE_SEND_BEGIN 101 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/

            //DONT FORGET TO HASH
            setDH(0);

            /* IBC_PRESERVE_SEND_END 101 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
        }
        break;
        /* IBC_MESSAGE_END 101 0 0 */
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
            int SonicFront = EEPROM.read(EEPROM_SONIC_FRONT_VALUE);
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
            recv(buffr181, 0);

            //DONT FORGET TO HASH
            setDH(createDH(buffr181, 0));

            /* IBC_PRESERVE_RECV_END 181 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

            char datahash = recv();
            send(sstat);

            /*Send exactly 2 bytes in the following                  */
            /*Also calculate their data hash along the way by                   */
            /*  xoring all bytes together once                                  */
            /*  or use the provided function createDH(..)                   */
            /* Make the hash public to the IBC by setDH(Your DATAHASH HERE) */
            /* IBC_PRESERVE_SEND_BEGIN 181 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
            int SonicBack = EEPROM.read(EEPROM_SONIC_BACK_VALUE);
            byte * bpSB = (byte*)&SonicBack;

            for (int i = 0; i < 2; i++) { send(bpSB[i]); }
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
            byte parityMag = EEPROM.read(EEPROM_MAGNETOMETER_PARITY);
            int Mag = EEPROM.read(EEPROM_MAGNETOMETER_VALUE);

            send(parityMag);

            byte * bpM = (byte*)&Mag;
            send(bpM, 2);

            //DONT FORGET TO HASH
            setDH(createDH(&parityMag, 1, createDH(bpM, 2)));

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
            byte parityXacc = EEPROM.read(EEPROM_ACCELEROMETER_X_PARITY);
            int Xaccel = EEPROM.read(EEPROM_ACCELEROMETER_X_VALUE);
            byte parityYacc = EEPROM.read(EEPROM_ACCELEROMETER_Y_PARITY);
            int Yaccel = EEPROM.read(EEPROM_ACCELEROMETER_Y_VALUE);

            send(parityXacc);
            byte * bpXacc = (byte*)&Xaccel;
            send(bpXacc, 2);
            send(parityYacc);
            byte * bpYacc = (byte*)&Yaccel;
            send(bpYacc, 2);

            //DONT FORGET TO HASH
            setDH(createDH(&parityXacc, 1, createDH(bpXacc, 2, createDH(&parityYacc, 1, createDH(bpYacc, 2)))));

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

            byte buff254[8] = { 1,2,3,4,5,6,7,count };

            send(buff254, 8);

            //DONT FORGET TO HASH
            setDH(8);

            if (count < 9) {
                count++;
            }
            else {
                count = 0;
            }

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
            recv(buffr253, 16);

            //DONT FORGET TO HASH
            setDH(createDH(buffr253, 16));

            /* IBC_PRESERVE_RECV_END 253 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

            char datahash = recv();
            send(sstat);

            /*Send exactly 4 bytes in the following                  */
            /*Also calculate their data hash along the way by                   */
            /*  xoring all bytes together once                                  */
            /*  or use the provided function createDH(..)                   */
            /* Make the hash public to the IBC by setDH(Your DATAHASH HERE) */
            /* IBC_PRESERVE_SEND_BEGIN 253 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/

            char message[4] = "me2";
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