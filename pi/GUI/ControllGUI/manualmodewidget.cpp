#include "manualmodewidget.h"

ManualModeWidget::ManualModeWidget(QWidget *parent, Alert *alertThread) : QWidget(parent)
{
    this->alertThread = alertThread;
}
