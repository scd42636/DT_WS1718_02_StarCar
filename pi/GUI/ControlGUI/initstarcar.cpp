#include "initstarcar.h"

InitStarCar::InitStarCar(Alert *alertThread, IBC **IBCPointer)
{
    this->alertThread = alertThread;
    this->IBCPointer = IBCPointer;
}

InitStarCar::InitStarCar(Alert *alertThread, SerialPort **serialPort, StreamSerialProtocol **protocol, message **msg)
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
    *msg = new message();
    (*msg)->Mode = 0;
    (*protocol)->send();
/*
    struct payload
    {
        uint8_t Mode;
        uint8_t Request;

        uint32_t DistanceFront;
        uint32_t DistanceBack;

        uint8_t DirectionParity;
        uint32_t DirectionValue;

        uint8_t AccelerationXParity;
        uint32_t AccelerationXValue;

        uint8_t AccelerationYParity;
        uint32_t AccelerationYValue;
    } __attribute__((packed)) mess;

    SerialPort serial = SerialPort("/dev/ttyUSB0");
    serial.config();
    int fd = serial.fd;
    StreamSerialProtocol prot = StreamSerialProtocol(fd,(uint8_t*)&mess,
                                                     sizeof(mess));
    prot.send();
    prot.send();
    prot.send();
*/
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
