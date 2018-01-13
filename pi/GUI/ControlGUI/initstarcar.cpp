#include "initstarcar.h"

InitStarCar::InitStarCar(Alert *alertThread, StarCarProtocol **starcarprotocol)
{
    this->alertThread = alertThread;
    this->starcarprotocol = starcarprotocol;
}

void InitStarCar::startProcess(){

#ifdef Q_OS_LINUX

    *starcarprotocol = new StarCarProtocol();

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
