#ifndef INITSTARCAR_H
#define INITSTARCAR_H

#define IBCNOTWORKING

#include <QObject>
#include <QThread>
#include <alert.h>

#include "../../IBP/IBC.hpp"
#include <../SerialProtocol/StreamSerialProtocol.h>
#include <../SerialProtocol/SerialPort.hpp>
#include <starcar.h>


class InitStarCar : public QObject {

    Q_OBJECT

public:

    InitStarCar(Alert *alertThread, IBC **IBCPointer);
    InitStarCar(Alert *alertThread, SerialPort **serialPort, StreamSerialProtocol **protocol, message *msg);
    ~InitStarCar();

public slots:

    void startProcess();
    void finishInitStarCar();

signals:

    void finished();
    void pushProcessBar();

private:

    // Thread
    Alert       *alertThread;

    IBC         **IBCPointer;
    Serial      **SerialPortArduino;


    SerialPort  **serialPort;
    StreamSerialProtocol **protocol;
    message         *msg;

};

#endif // INITSTARCAR_H
