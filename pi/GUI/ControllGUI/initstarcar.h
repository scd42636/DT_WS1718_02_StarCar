#ifndef INITSTARCAR_H
#define INITSTARCAR_H

#include <QObject>
#include <QThread>
#include <alert.h>
#include <../../IBP/Serial.hpp>

class InitStarCar : public QObject {

    Q_OBJECT

public:

    InitStarCar(Alert *alertThread);
    ~InitStarCar();

public slots:

    void startProcess();
    void finishInitStarCar();

signals:

    void finished();
    void pushProcessBar();

private:

    Alert       *alertThread;
};

#endif // INITSTARCAR_H
