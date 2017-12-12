#include "clockcontrolmode.h"

ClockControlMode::ClockControlMode(Alert *alertThread)
{
    this->alertThread = alertThread;
}

void ClockControlMode::startProcess(){

    qDebug("ClockControlModeThread started");
    emit finished();
}

void ClockControlMode::finishAutomaticMode(){

}
