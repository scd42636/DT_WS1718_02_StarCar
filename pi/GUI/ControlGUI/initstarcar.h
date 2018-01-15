#ifndef INITSTARCAR_H
#define INITSTARCAR_H

#define IBCNOTWORKING

#include <QObject>
#include <QThread>
#include <alert.h>

#ifndef IBCNOTWORKING

   #include "../IBP/IBC.hpp"

#else

   #include "../StarCarSerialProtocol/StarcarProtocol.h"

#endif

class InitStarCar : public QObject {

    Q_OBJECT

public:

#ifndef IBCNOTWORKING

   InitStarCar(Alert *alertThread, IBC **IBCPointer);

#else

   InitStarCar(Alert *alertThread, StarCarProtocol **starcarprotocol);

#endif


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

#ifndef IBCNOTWORKING

    // IBC
    IBC             **IBCPointer;

#else

    //Protocol
    StarCarProtocol **starcarprotocol;

#endif

};

#endif // INITSTARCAR_H
