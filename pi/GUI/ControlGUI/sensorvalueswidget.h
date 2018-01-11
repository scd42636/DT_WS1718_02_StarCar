#ifndef SENSORVALUESWIDGET_H
#define SENSORVALUESWIDGET_H

#define PI

#include <QObject>
#include <QWidget>
#include <alert.h>
#include <QVBoxLayout>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <../../IBP/IBC.hpp>
#include <../../IBP/IBC_Packet.hpp>
#include <../SerialProtocol/SerialPort.hpp>
#include <../SerialProtocol/StreamSerialProtocol.h>
#include <threadlidar.h>

class SensorValuesWidget : public QWidget
{
    Q_OBJECT

public:

    explicit SensorValuesWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr,
                                QString pButtonGoBackText = "Zurück zur Moduswahl",
                                IBC *IBCPointer = nullptr);

signals:

    void removeWindowfromStack();

public slots:

private slots:

    void slotpButtonGoBackPushed();
    void slotQuerySensorValues();

private:

    // Thread
    Alert           *alertThread;
    ThreadLidar     *threadLidar;

    // QVBoxLayout
    QVBoxLayout     *vBox1;
    QVBoxLayout     *vBoxLabelDescription;
    QVBoxLayout     *vBoxLabelValues;

    // QHBoxLayout
    QHBoxLayout     *hBox1;

    // QPushButton
    QPushButton     *pButtonGoBack;

    // QLabel
    QLabel          *lblUltraFront;
    QLabel          *lblUltraBack;
    QLabel          *lblcompass;
    QLabel          *lblacceleration;
    QLabel          *lblUWB;

    QLabel          *lblUltraFrontValue;
    QLabel          *lblUltraBackValue;
    QLabel          *lblcompassValue;
    QLabel          *lblaccelerationValue;
    QLabel          *lblUWBValue;

    // QVector
    QVector<QLabel *> lblValues;

    // QString
    QString         pButtonGoBackText;

    // IBC
    IBC             *IBCPointer;
    SerialPort      *serial;
    StreamSerialProtocol *protocol;

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

    struct payload
    {
        StarCarMode Mode;
        StarCarSensorRequest Request;

        uint16_t DistanceFront;
        uint16_t DistanceBack;

        uint8_t DirectionParity;
        uint16_t DirectionValue;

        uint8_t AccelerationXParity;
        uint16_t AccelerationXValue;

        uint8_t AccelerationYParity;
        uint16_t AccelerationYValue;
    } __attribute__((packed)) message;

    // QTimer
    QTimer          *lidarTimer;

#ifdef Q_OS_LINUX

    Packet          *packetUltrafront;
    Packet          *packetUltraback;
    Packet          *packetCompass;
    Packet          *packetAcceleration;
    Packet          *packetUWB;
    Packet          *packetTest;

    Inbox           *iUltraFront;
    Inbox           *iUltraBack;
    Inbox           *iCompass;
    Inbox           *iAcceleration;
    Inbox           *iUWB;
    Inbox           *test;

#endif

    //QTimer
    QTimer          *QuerySensorValuesTimer;

    // Method
    void generateLayout();
    void setupConnects();
    void generateStyle();

    QString getMesureValue(Inbox *inbox);

    template<typename TData>
    void ReadData(TData* data, Packet packet)
    {
        size_t dataSize = sizeof(TData);

        if (dataSize < packet.contentsize())
            dataSize = packet.contentsize();

        memcpy(data, packet.content(), dataSize);
    }
};

#endif // SENSORVALUESWIDGET_H
