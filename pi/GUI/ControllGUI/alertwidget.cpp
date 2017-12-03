#include "alertwidget.h"

AlertWidget::AlertWidget(QWidget *parent) : QWidget(parent)
{

    this->setStyleSheet("QWidget{"
                        "background-color: red;}");
}



AlertWidget::~AlertWidget(){

}
