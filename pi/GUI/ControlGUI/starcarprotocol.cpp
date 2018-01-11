#include "starcarprotocol.h"

StarCarProtocol::StarCarProtocol()
{
    message.Mode = CarMode_None;
    message.Request = CarSensorRequest_None;

    message.DistanceFront = 0;
    message.DistanceBack = 0;

    message.DirectionParity = 0;
    message.DirectionValue = 0;

    message.AccelerationXParity = 0;
    message.AccelerationXValue = 0;

    message.AccelerationYParity = 0;
    message.AccelerationYValue = 0;

    initSerialPort();
    initProtocol();

}

StarCarProtocol::~StarCarProtocol(){

    delete this->serialPort;
}

void StarCarProtocol::initSerialPort(){

    this->serialPort = new SerialPort("/dev/ttyUSB0");
    this->serialPort->config();
    this->fd = this->serialPort->getFD();
}

void StarCarProtocol::initProtocol(){

    this->protocol = new StreamSerialProtocol(this->fd, (uint8_t *)&this->message, sizeof(message));
}

void StarCarProtocol::setMode(StarCarMode mode){

    this->message.Mode = mode;
}

int StarCarProtocol::getMode(){

    return message.Mode;
}

void StarCarProtocol::setRequest(StarCarSensorRequest request){

    this->message.Request = request;
}

int StarCarProtocol::getRequest(){

    return message.Request;
}

void StarCarProtocol::send(){

    this->protocol->send();
}

int StarCarProtocol::receive(){

    uint8_t receiveState = this->protocol->receive();

    return (int)receiveState;
}

int StarCarProtocol::getDistanceFront(){

    return message.DistanceFront;
}

int StarCarProtocol::getDistanceBack(){

    return message.DistanceBack;
}

int StarCarProtocol::getCompass(){

    if(message.DirectionParity == 1)
    {
        return -(message.DirectionValue);

    }else{

        return message.DirectionValue;
    }
}

int StarCarProtocol::getAccelerationX(){

    if(message.AccelerationXParity == 1)
    {
        return -(message.AccelerationXValue);

    }else{

        return message.AccelerationXValue;
    }
}

int StarCarProtocol::getAccelerationY(){

    if(message.AccelerationYParity == 1)
    {
        return -(message.AccelerationYValue);

    }else{

        return message.AccelerationYValue;
    }
}
