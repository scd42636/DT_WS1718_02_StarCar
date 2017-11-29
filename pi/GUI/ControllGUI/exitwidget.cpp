#include "exitwidget.h"

ExitWidget::ExitWidget(QWidget *parent) : QWidget(parent)
{
    generateLayout();
    setupConnect();
    generateStyle();
}

void ExitWidget::generateLayout(){

    vBox1 = new QVBoxLayout(this);
    pButtonBack = new QPushButton();
    pButtonRestart = new QPushButton();
    pButtonShutdown = new QPushButton();

    vBox1->addSpacing(83);
    vBox1->addWidget(pButtonBack);
    vBox1->addSpacing(5);
    vBox1->addWidget(pButtonRestart);
    vBox1->addSpacing(5);
    vBox1->addWidget(pButtonShutdown);
    vBox1->addSpacing(83);
}

void ExitWidget::generateStyle(){

    /*pButtonBack->setStyleSheet("QPushButton { "
                               "border-radius:  10px;"
                               "border-width:   3px;"
                               "border-color:   black;"
                               "border-style:   solid; }");*/

    pButtonRestart->setStyleSheet("QPushButton { "
                               "border-radius:  10px;"
                               "border-width:   3px;"
                               "border-color:   black"
                               "border-style:   solid }");

    pButtonShutdown->setStyleSheet("QPushButton { "
                               "border-radius:  10px;"
                               "border-width:   3px;"
                               "border-color:   black"
                               "border-style:   solid }");
}

void ExitWidget::setupConnect(){

    connect(pButtonBack,     SIGNAL(clicked(bool)), this, SLOT(goBack()));
    connect(pButtonRestart,  SIGNAL(clicked(bool)), this, SLOT(restartPi()));
    connect(pButtonShutdown, SIGNAL(clicked(bool)), this, SLOT(shutdownPi()));
}

void ExitWidget::goBack(){

    emit removeWindowformStack();
}

void ExitWidget::restartPi(){


}

void ExitWidget::shutdownPi(){


}
