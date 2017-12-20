#ifndef INITSTARCAR_H
#define INITSTARCAR_H

#include <QObject>
#include <QThread>
#include <alert.h>
#include <../../IBP/IBC.hpp>

class InitStarCar : public QObject {

    Q_OBJECT

public:

    InitStarCar(Alert *alertThread, IBC *IBCPointer, Serial *SerialPortArduino);
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

    // IBC

    IBC         *IBCPointer;
    Serial      *SerialPortArduino;
};

#endif // INITSTARCAR_H
