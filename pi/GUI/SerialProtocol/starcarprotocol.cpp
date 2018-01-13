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

    initSerialPort();
    //initProtocol();

}

StarCarProtocol::~StarCarProtocol(){

    //delete this->serialPort;
    delete this->serial;
}

void StarCarProtocol::initSerialPort(){

    /*
    this->serialPort = new SerialPort("/dev/ttyUSB0");
    this->serialPort->config();
    this->fd = this->serialPort->getFD();
    */

    this->serial = new Serial("/dev/ttyUSB0");
}

void StarCarProtocol::initProtocol(){

    this->protocol = new StreamSerialProtocol(this->fd, (uint8_t *)&this->message, sizeof(message));
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

    //this->protocol->send();
    uint8_t data = 55;
    serial->send(&data,sizeof(data));
}

int StarCarProtocol::receive(){

    /*uint8_t receiveState = this->protocol->receive();

    qDebug("%d \n",receiveState);

    if (receiveState == ProtocolState::SUCCESS){

        time(&lastPackage);
    }

   time(&now);

   if (difftime(now, lastPackage) > 5){

       printf("Retrying the last package...\n");

       // If something went bad, retry the last package
       protocol->send();
       time(&lastPackage);
   }

    return (int)receiveState;*/

    uint8_t buffer;

    serial->recv(&buffer,sizeof(message));
    memcpy(&message,buffer,sizeof(message));
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
