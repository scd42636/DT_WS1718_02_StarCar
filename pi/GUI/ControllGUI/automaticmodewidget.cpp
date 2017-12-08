#include "automaticmodewidget.h"

AutomaticModeWidget::AutomaticModeWidget(QWidget *parent, Alert *alertThread) : QWidget(parent)
{
    this->alertThread = alertThread;

    QThread *thread = new QThread;
    automaticMode = new AutomaticMode(alertThread);
    automaticMode->moveToThread(thread);

    connect(thread, SIGNAL(started()), automaticMode, SLOT(startProcess()));
    connect(automaticMode, SIGNAL(finished()), thread, SLOT(quit()));
    connect(automaticMode, SIGNAL(finished()), automaticMode, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}
