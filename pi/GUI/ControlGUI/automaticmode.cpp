#include "automaticmode.h"

AutomaticMode::AutomaticMode(Alert *alertThread)
{
    this->alertThread = alertThread;
}

void AutomaticMode::startProcess(){

    qDebug("AutomaticModeThread");
    emit finished();
}

void AutomaticMode::finishAutomaticMode(){

}
