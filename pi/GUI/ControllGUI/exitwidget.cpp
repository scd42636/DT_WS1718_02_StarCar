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

    vBox1->addWidget(pButtonBack);
    vBox1->addSpacing(5);
    vBox1->addWidget(pButtonRestart);
    vBox1->addSpacing(5);
    vBox1->addWidget(pButtonShutdown);
}

void ExitWidget::generateStyle(){

    vBox1->setAlignment(Qt::AlignHCenter);

    pButtonBack->setText("Zurück");
    pButtonBack->setObjectName("QPushButton1");

    pButtonRestart->setText("Neustart");
    pButtonShutdown->setText("Beenden");

    pButtonBack->setMinimumWidth(160);
    pButtonBack->setMinimumHeight(30);
    pButtonRestart->setMaximumWidth(160);
    pButtonRestart->setMinimumHeight(30);
    pButtonShutdown->setMaximumWidth(160);
    pButtonShutdown->setMinimumHeight(30);

    this->setStyleSheet("QPushButton{"
                        "color: green;"
                        "font-family: TimesNewRoman;"
                        "font-style: normal;"
                        "font-size: 10pt;"
                        "font-weight: bold;}");
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

    this->parentWidget()->parentWidget()->parentWidget()->close();
}
