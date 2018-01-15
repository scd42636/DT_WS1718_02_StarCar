#ifndef THREADLIDAR_H
#define THREADLIDAR_H

#include <QObject>
#include "alert.h"
#include "../lidar/lidar.h"

class ThreadLidar : public QObject{

    Q_OBJECT

public:

    ThreadLidar(Alert *alertThread);
    ~ThreadLidar();

public slots:

    void startProcess();
    void finishLidar();

signals:

    void finished();

private:

    // Thread
    Alert           *alertThread;

    lidar           *Lidar;
    bool            measure = true;
};

#endif // THREADLIDAR_H
