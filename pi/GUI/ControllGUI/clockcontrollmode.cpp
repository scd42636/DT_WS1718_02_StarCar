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

// copy this to clockcontrolmodewidget.cpp into constructor to use this thread

/*
    QThread *thread = new QThread;
    manualMode= new ClockControlMode(alertThread);
    manualMode->moveToThread(thread);

    connect(thread, SIGNAL(started()), manualMode, SLOT(startProcess()));
    connect(manualMode, SIGNAL(finished()), thread, SLOT(quit()));
    connect(manualMode, SIGNAL(finished()), manualMode, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
*/
