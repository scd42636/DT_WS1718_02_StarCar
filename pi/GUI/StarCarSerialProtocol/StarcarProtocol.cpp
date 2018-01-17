#include "StarcarProtocol.h"

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


    this->serial = new SerialPort("/dev/ttyUSB1");
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

bool StarCarProtocol::messagevalid(){

    unsigned short int CRC = 0;
    CRC ^= message.Mode;
    CRC ^= message.Request;
    CRC ^= message.DistanceFront;
    CRC ^= message.DistanceBack;
    CRC ^= message.DirectionParity;
    CRC ^= message.DirectionValue;
    CRC ^= message.AccelerationXParity;
    CRC ^= message.AccelerationXValue;
    CRC ^= message.AccelerationYParity;
    CRC ^= message.AccelerationYValue;

    if(CRC == message.crc)
    {
        return true;

    }else{

        return false;
    }
}

void StarCarProtocol::writetoFile(QString filePath, QString value){

    QFile file(filePath);

    if(file.open(QIODevice::WriteOnly | QIODevice::Append)){

        QTextStream stream (&file);
        stream << value << endl;
        file.close();

    }else{

        printf("Konnte file nicht öffnen...");
        //alertThread->fireError("Could not open File" + filePath);
    }

}

int StarCarProtocol::getDistanceFront(){

    writetoFile("/home/pi/SensorOutput/UltraVorne.txt", QString::number((int)message.DistanceFront));

    return message.DistanceFront;
}

int StarCarProtocol::getDistanceBack(){

    writetoFile("/home/pi/SensorOutput/UltraHinten.txt", QString::number((int)message.DistanceBack));

    return message.DistanceBack;
}

int StarCarProtocol::getCompass(){

    if(message.DirectionParity == 1)
    {
        writetoFile("/home/pi/SensorOutput/Compass.txt", QString::number(-(int)message.DirectionValue));
        return -(message.DirectionValue);

    }else{

        writetoFile("/home/pi/SensorOutput/Compass.txt", QString::number((int)message.DirectionValue));
        return message.DirectionValue;
    }
}

int StarCarProtocol::getAccelerationX(){

    if(message.AccelerationXParity == 1)
    {
        writetoFile("/home/pi/SensorOutput/BeschleunigungX.txt", QString::number(-(int)message.AccelerationXValue));
        return -(message.AccelerationXValue);

    }else{

        writetoFile("/home/pi/SensorOutput/BeschleunigungX.txt", QString::number((int)message.AccelerationXValue));
        return message.AccelerationXValue;
    }
}

int StarCarProtocol::getAccelerationY(){

    if(message.AccelerationYParity == 1)
    {
        writetoFile("/home/pi/SensorOutput/BeschleunigungY.txt", QString::number(-(int)message.AccelerationYValue));
        return -(message.AccelerationYValue);

    }else{

        writetoFile("/home/pi/SensorOutput/BeschleunigungY.txt", QString::number((int)message.AccelerationYValue));
        return message.AccelerationYValue;
    }
}
