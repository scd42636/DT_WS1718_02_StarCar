#include "threadlidar.h"

ThreadLidar::ThreadLidar(Alert *alertThread)
{
    this->alertThread = alertThread;
    Lidar = new lidar();

}

void ThreadLidar::startProcess(){

    while(measure){

        int retval = Lidar->get_measurement("testfile.txt");

        if(retval != 0){

            alertThread->fireError("Lidar not working!");
            qDebug("Lidar not working!");
        }

        this->thread()->sleep(1);
    }

    emit finished();
}

void ThreadLidar::finishLidar(){

    measure = false;
}

ThreadLidar::~ThreadLidar(){

    delete Lidar;
    this->alertThread->fireWarning("Lidar finished");
}
