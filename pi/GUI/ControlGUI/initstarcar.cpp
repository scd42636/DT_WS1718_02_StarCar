#include "initstarcar.h"

//#define IBCNOTWORKING

InitStarCar::InitStarCar(Alert *alertThread, IBC **IBCPointer, Serial **SerialPortArduino)
{
    this->alertThread = alertThread;
    this->IBCPointer = IBCPointer;
    this->SerialPortArduino = SerialPortArduino;
}

void InitStarCar::startProcess(){

#ifdef Q_OS_LINUX

#ifdef IBCNOTWORKING

    *SerialPortArduino = new Serial("/dev/ttyUSB0");

#else
    *SerialPortArduino = nullptr;
    *IBCPointer = new IBC("/dev/ttyUSB0","../../IBC_config.cfg");

#endif

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
