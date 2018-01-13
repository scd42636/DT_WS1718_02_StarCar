#include "starcarprotocol.h"

StarCarProtocol::StarCarProtocol()
{
    message.Mode = 0;
    message.Request = 0;

    message.DistanceFront = 0;
    message.DistanceBack = 0;

    message.DirectionParity = 0;
    message.DirectionValue = 0;

    message.AccelerationXParity = 0;
    message.AccelerationXValue = 0;

    message.AccelerationYParity = 0;
    message.AccelerationYValue = 0;

}

StarCarProtocol::~StarCarProtocol(){

    delete this->serial;
}

void StarCarProtocol::initSerialPort(){


    this->serial = new Serial("/dev/ttyUSB0");
}


void StarCarProtocol::setMode(int mode){

    this->message.Mode = mode;
}

int StarCarProtocol::getMode(){

    return message.Mode;
}

void StarCarProtocol::setRequest(int request){

    this->message.Request = request;
}

int StarCarProtocol::getRequest(){

    return message.Request;
}

void StarCarProtocol::send(){

    serial->send(&message.Request,sizeof(message.Request));
}

void StarCarProtocol::receive(){

    serial->recv(&message,sizeof(message));
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
