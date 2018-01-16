#ifndef STARCARPROTOCOL_H
#define STARCARPROTOCOL_H

#include <QObject>
#include "SerialPort.hpp"
#include <QFile>
#include <QTextStream>
#include <QThread>
#include <thread>

enum StarCarMode
{
    CarMode_None = 0,
    CarMode_Controller = 1,
    CarMode_Watch = 2
};

enum StarCarSensorRequest
{
    CarSensorRequest_None = 0,
    CarSensorRequest_Sonic = 1,
    CarSensorRequest_Magnet = 2,
    CarSensorRequest_Accelerator = 4,
    CarSensorRequest_All = 7
};

class StarCarProtocol : public QObject
{
    Q_OBJECT

public:

    explicit StarCarProtocol();
    ~StarCarProtocol();

    void initSerialPort();

    void setMode(int mode);
    void setRequest(int request);
    void send();
    void receive();

    int getDistanceFront();
    int getDistanceBack();
    int getCompass();
    int getAccelerationX();
    int getAccelerationY();

    int getMode();
    int getRequest();

    bool messagevalid();

private:

    SerialPort                      *serial;

    struct payload
    {
        unsigned char Mode = 0;
        unsigned char Request = 0;

        unsigned short int DistanceFront = 0;
        unsigned short int  DistanceBack = 0;

        unsigned char DirectionParity = 0;
        unsigned short int DirectionValue = 0;

        unsigned char AccelerationXParity = 0;
        unsigned short int  AccelerationXValue = 0;

        unsigned char AccelerationYParity = 0;
        unsigned short int AccelerationYValue = 0;

        unsigned short int crc = 0;

    }__attribute__((packed))  message;

    void writetoFile(QString filePath, QString value);

    bool runReceive = false;
    bool runSend = false;

    std::thread readThread;
    std::thread sendThread;

    void threadSend();
    void threadReceive();
};

#endif // STARCARPROTOCOL_H
