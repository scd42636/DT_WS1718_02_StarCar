#include "automaticmodewidget.h"

AutomaticModeWidget::AutomaticModeWidget(QWidget *parent, Alert *alertThread) : QWidget(parent)
{
    this->alertThread = alertThread;
    alertThread->fireError();
}
