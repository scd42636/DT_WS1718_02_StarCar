#ifndef STARCARPROTOCOL_H
#define STARCARPROTOCOL_H

#include <QObject>
#include "Serial.hpp"
#include <QFile>
#include <QTextStream>

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

private:

    Serial                      *serial;

    struct payload
    {
        uint8_t Mode = 0;
        uint8_t Request = 0;

        uint16_t DistanceFront = 0;
        uint16_t DistanceBack = 0;

        uint8_t DirectionParity = 0;
        uint16_t DirectionValue = 0;

        uint8_t AccelerationXParity = 0;
        uint16_t AccelerationXValue = 0;

        uint8_t AccelerationYParity = 0;
        uint16_t AccelerationYValue = 0;

    }__attribute__((packed))  message;

    void writetoFile(QString filePath, QString value);
};

#endif // STARCARPROTOCOL_H
