#include "alertwidget.h"


AlertWidget::AlertWidget(QWidget *parent, Alert *alertThread) : QWidget(parent)
{
    this->alertThread = alertThread;
}



AlertWidget::~AlertWidget(){

}
