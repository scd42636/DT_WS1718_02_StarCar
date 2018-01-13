#ifndef INITSTARCAR_H
#define INITSTARCAR_H

#include <QObject>
#include <QThread>
#include <alert.h>
#include <../StarCarSerialProtocol/starcarprotocol.h>

class InitStarCar : public QObject {

    Q_OBJECT

public:

    InitStarCar(Alert *alertThread, StarCarProtocol **starcarprotocol);
    ~InitStarCar();

public slots:

    void startProcess();
    void finishInitStarCar();

signals:

    void finished();
    void pushProcessBar();

private:

    // Thread
    Alert           *alertThread;

    // IBC
    StarCarProtocol **starcarprotocol;
};

#endif // INITSTARCAR_H
