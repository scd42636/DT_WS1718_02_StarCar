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
    initProtocol();

}

StarCarProtocol::~StarCarProtocol(){

    delete this->serialPort;
}

void StarCarProtocol::initSerialPort(){

    int serial = open("/dev/ttyUSB0",O_RDWR | O_NOCTTY | O_NDELAY);

    if (serial == -1){

        printf("Failed to open serial port.\n");
   }

   struct termios options;

    tcgetattr(serial, &options);
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(serial, TCIFLUSH);
    tcsetattr(serial, TCSANOW, &options);


    protocol =  new StreamSerialProtocol(serial, (uint8_t*)&message, sizeof(message));

    /*this->serialPort = new SerialPort("/dev/ttyUSB0");
    this->serialPort->config();
    this->fd = this->serialPort->getFD();*/
}

void StarCarProtocol::initProtocol(){

    //this->protocol = new StreamSerialProtocol(this->fd, (uint8_t *)&this->message, sizeof(message));
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

    ++message.DistanceFront;

    this->protocol->send();
}

int StarCarProtocol::receive(){

    uint8_t receiveState = this->protocol->receive();

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
