#include "initstarcar.h"
#include "../../IBP/IBC.hpp"

InitStarCar::InitStarCar(Alert *alertThread)
{
    this->alertThread = alertThread;
}

void InitStarCar::startProcess(){

    for(int i =0; i < 15; i++){

        emit pushProcessBar();
    }

    //IBC *test = new IBC("test","../../IBC_config.cfg");

    emit finished();
}

void InitStarCar::finishInitStarCar(){

}

InitStarCar::~InitStarCar(){

}
