#include "initstarcar.h"

InitStarCar::InitStarCar(Alert *alertThread)
{
    this->alertThread = alertThread;
}

void InitStarCar::startProcess(){

    for(int i =0; i < 15; i++){

        emit pushProcessBar();
    }

    emit finished();
}

void InitStarCar::finishInitStarCar(){

}

InitStarCar::~InitStarCar(){

}
