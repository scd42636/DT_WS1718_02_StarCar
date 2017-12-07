#include "initstarcar.h"

InitStarCar::InitStarCar(Alert *alertThread)
{
    this->alertThread = alertThread;
}

void InitStarCar::startProcess(){

    for(int i =0; i < 15; i++){

        emit pushProcessBar();
    }

    alertThread->fireError("test");
    alertThread->fireWarning("test2");

    emit finished();
}

void InitStarCar::finishInitStarCar(){

}

InitStarCar::~InitStarCar(){

}
