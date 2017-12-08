#include "manualmodewidget.h"

ManualModeWidget::ManualModeWidget(QWidget *parent, Alert *alertThread) : QWidget(parent)
{
    this->alertThread = alertThread;

    QThread *thread = new QThread;
    manualMode= new ManualMode(alertThread);
    manualMode->moveToThread(thread);

    connect(thread, SIGNAL(started()), manualMode, SLOT(startProcess()));
    connect(manualMode, SIGNAL(finished()), thread, SLOT(quit()));
    connect(manualMode, SIGNAL(finished()), manualMode, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}
