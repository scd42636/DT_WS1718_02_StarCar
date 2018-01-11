#include "initstarcar.h"

InitStarCar::InitStarCar(Alert *alertThread, IBC **IBCPointer)
{
    this->alertThread = alertThread;
    this->IBCPointer = IBCPointer;
}

InitStarCar::InitStarCar(Alert *alertThread, SerialPort **serialPort, StreamSerialProtocol **protocol, message *msg)
{
    this->alertThread = alertThread;
    this->serialPort = serialPort;
    this->protocol = protocol;
    this->msg = msg;
}

void InitStarCar::startProcess(){

#ifdef Q_OS_LINUX

#ifndef IBCNOTWORKING

    *IBCPointer = new IBC("/dev/ttyUSB0","/home/pi/DT_WS1718_02_StarCar/pi/IBP/IBC_config.cfg");
#else

    *serialPort = new SerialPort("/dev/ttyUSB0");
    (*serialPort)->config();
    int fd = (*serialPort)->fd;

    *protocol = new StreamSerialProtocol(fd, (uint8_t*)msg, sizeof(msg));
    msg->Mode = CarMode_Controller;

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
