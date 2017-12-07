#include "manualmode.h"

ManualMode::ManualMode(Alert *alertThread)
{
    this->alertThread = alertThread;
}

void ManualMode::startProcess(){

    qDebug("ManualModeThread started");
    emit finished();
}

void ManualMode::finishAutomaticMode(){

}
