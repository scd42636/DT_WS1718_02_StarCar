#ifndef SENSORVALUESWIDGET_H
#define SENSORVALUESWIDGET_H

#include <QObject>
#include <QWidget>
#include <alert.h>
#include <QVBoxLayout>
#include <QLabel>
#include <../../IBP/IBC.hpp>
#include <../../IBP/IBC_Packet.hpp>

class SensorValuesWidget : public QWidget
{
    Q_OBJECT

public:

    explicit SensorValuesWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr, QString pButtonGoBackText = "Zurück zur Moduswahl", IBC *IBCPointer = nullptr);

signals:

    void removeWindowfromStack();

public slots:

private slots:

    void pButtonGoBackPushed();
    void slotQuerySensorValues();

private:

    // Thread
    Alert           *alertThread;

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

#ifdef Q_OS_LINUX

    Packet          *packetUltrafront;
    Packet          *packetUltraback;
    Packet          *packetCompass;
    Packet          *packetAcceleration;

    Inbox           *testInbox;

/*
    Inbox           *iUltraFront;
    Inbox           *iUltraBack;
    Inbox           *iCompass;
    Inbox           *iAcceleration;
*/
#endif

    //QTimer
    QTimer          *QuerySensorValuesTimer;

    // Methode
    void generateLayout();
    void setupConnects();
    void generateStyle();

};

#endif // SENSORVALUESWIDGET_H
