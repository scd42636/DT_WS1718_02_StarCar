#ifndef SENSORVALUESWIDGET_H
#define SENSORVALUESWIDGET_H

#define PI
#define IBCNOTWORKING

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
#include <../SerialProtocol/StreamSerialProtocol.h>
#include <../SerialProtocol/SerialPort.hpp>
#include <starcar.h>
#include <threadlidar.h>
#include <starcarprotocol.h>

class SensorValuesWidget : public QWidget
{
    Q_OBJECT

public:

    explicit SensorValuesWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr,
                                QString pButtonGoBackText = "Zurück zur Moduswahl",
                                IBC *IBCPointer = nullptr);

    explicit SensorValuesWidget(message *msg, QWidget *parent = nullptr, Alert *alertThread = nullptr,
                                QString pButtonGoBackText = "Zurück zur Moduswahl",
                                StreamSerialProtocol *protocol = nullptr, SerialPort *serialPort = nullptr);

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

    // QTimer
    QTimer          *lidarTimer;
    QTimer          *QuerySensorValuesTimer;

    // Method
    void generateLayout();
    void setupConnects();
    void generateStyle();

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


    QString getMesureValue(Inbox *inbox);

    template<typename TData>
    void ReadData(TData* data, Packet packet)
    {
        size_t dataSize = sizeof(TData);

        if (dataSize < packet.contentsize())
            dataSize = packet.contentsize();

        memcpy(data, packet.content(), dataSize);
    }

    IBC             *IBCPointer;

    StreamSerialProtocol *protocol;
    SerialPort *serialPort;
    message *msg;

    StarCarProtocol     *starcarProtocol;


#endif

};

#endif // SENSORVALUESWIDGET_H
