#include "initstarcar.h"

InitStarCar::InitStarCar(Alert *alertThread)
{
    this->alertThread = alertThread;
}

void InitStarCar::startProcess(){

    for(int i =0; i < 15; i++){

        emit pushProcessBar();
    }

    Serial *SerialPort =  new Serial("Arduino");
    SerialPort->send("test MSG",8);

    char *data;

    SerialPort->recv(&data,8);

    emit finished();
}

void InitStarCar::finishInitStarCar(){

}

InitStarCar::~InitStarCar(){

}
