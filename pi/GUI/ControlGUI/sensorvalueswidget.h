#ifndef SENSORVALUESWIDGET_H
#define SENSORVALUESWIDGET_H

#define IBCNOTWORKING

#include <QObject>
#include <QWidget>
#include <alert.h>
#include <QVBoxLayout>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <threadlidar.h>

#ifndef IBCNOTWORKING

   #include "../IBP/IBC.hpp"

#else

   #include "../StarCarSerialProtocol/StarcarProtocol.h"

#endif

class SensorValuesWidget : public QWidget
{
    Q_OBJECT

public:

#ifndef IBCNOTWORKING

    explicit SensorValuesWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr,
                                    QString pButtonGoBackText = "Zurück zur Moduswahl",
                                    IBC *IBCPointer = nullptr);

#else

    explicit SensorValuesWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr,
                                QString pButtonGoBackText = "Zurück zur Moduswahl",
                                StarCarProtocol *starcarprotocol = nullptr);
#endif


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

    //QTimer
    QTimer          *QuerySensorValuesTimer;

#ifndef IBCNOTWORKING

    typedef uint8_t InoByte_t;
    typedef int16_t InoInt_t;

    typedef struct StarCarSonicData_t
    {
        InoInt_t Value;

    } StarCarSonicData;

    typedef struct StarCarMagnetData_t
    {
        InoByte_t Parity;
        InoInt_t Value;

    } StarCarMagnetData;

    typedef struct StarCarAccelerationData_t
    {
        InoByte_t ParityOfXValue;
        InoInt_t  XValue;
        InoByte_t ParityOfYValue;
        InoInt_t  YValue;

    } StarCarAccelerationData;

    // IBC
    IBC             *IBCPointer;

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

#else

    //Protocol
    StarCarProtocol *starcarprotocol;

#endif

    // Method
    void generateLayout();
    void setupConnects();
    void generateStyle();

};

#endif // SENSORVALUESWIDGET_H
