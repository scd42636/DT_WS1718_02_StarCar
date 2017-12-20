#include "initstarcar.h"

InitStarCar::InitStarCar(Alert *alertThread, IBC **IBCPointer)
{
    this->alertThread = alertThread;
    this->IBCPointer = IBCPointer;
}

void InitStarCar::startProcess(){

#ifdef Q_OS_LINUX

    *IBCPointer = new IBC("/dev/ttyUSB0","../../IBC_config.cfg");

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
