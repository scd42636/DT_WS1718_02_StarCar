//--------------------------------------------------------------------------------------------------
// <copyright file="IbcDriver.h" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Robert Graf</author>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#pragma once
#include "../car/StarCar.h"

class IbcDriver
{
private:
    byte DH;
    int count = 0;

public:
    IbcDriver(int baud);
    void next(StarCar* car);

    void send(byte* d, int length);
    void send(byte d);
    int recv(byte*d, int length);
    byte recv();

    void setDH(byte dh);

    byte createDH(byte* b, int lenght);
    byte createDH(byte* b, int lenght, byte in);
};