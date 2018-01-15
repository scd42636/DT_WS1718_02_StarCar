#include "initstarcar.h"

#ifndef IBCNOTWORKING

InitStarCar::InitStarCar(Alert *alertThread, IBC **IBCPointer)
{
    this->alertThread = alertThread;
    this->IBCPointer = IBCPointer;
}

#else

InitStarCar::InitStarCar(Alert *alertThread, StarCarProtocol **starcarprotocol)
{
    this->alertThread = alertThread;
    this->starcarprotocol = starcarprotocol;
}

#endif


void InitStarCar::startProcess(){

    #ifndef IBCNOTWORKING

       *IBCPointer = new IBC("/dev/ttyACM0","/home/pi/DT_WS1718_02_StarCar/pi/IBP/IBC_config.cfg");

    #else

        *starcarprotocol = new StarCarProtocol();
        (*starcarprotocol)->initSerialPort();

    #endif

    for(int i =0; i < 15; i++){

        emit pushProcessBar();
    }

    emit finished();
}

void InitStarCar::finishInitStarCar(){

}

InitStarCar::~InitStarCar(){

}
