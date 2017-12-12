#include "operationmodewidget.h"

OperationModeWidget::OperationModeWidget(QWidget *parent, Alert *alertThread) : QWidget(parent)
{
    this->alertThread = alertThread;
    generateLayout();
    generateStyle();
    setupConnects();
}

void OperationModeWidget::generateLayout(){

    vBox1 = new QVBoxLayout(this);
    lblHeadline = new QLabel();
    //hBox1 = new QHBoxLayout();
    pButtonClockControlMode = new QPushButton();
    //pButtonAutomaticMode = new QPushButton();
    pButtonControllerControlMode = new QPushButton();

    vBox1->addWidget(lblHeadline);
    vBox1->addSpacing(20);
    //vBox1->addLayout(hBox1);
    vBox1->addWidget(pButtonClockControlMode);
    vBox1->addWidget(pButtonControllerControlMode);

    //hBox1->addWidget(pButtonClockControlMode);
    //hBox1->addSpacing(30);
    //hBox1->addWidget(pButtonAutomaticMode);
}

void OperationModeWidget::generateStyle(){

    vBox1->setAlignment(Qt::AlignVCenter);
    lblHeadline->setAlignment(Qt::AlignHCenter);
    //hBox1->setAlignment(Qt::AlignVCenter);

    lblHeadline->setText("Betriebsmodi Auswahl");


    pButtonClockControlMode->setText("Uhrsteuerung");
    pButtonControllerControlMode->setText("Controllersteuerung");
    //pButtonAutomaticMode->setText("Automatik");

    pButtonClockControlMode->setMinimumSize(20,20);
    //pButtonAutomaticMode->setMinimumSize(20,20);

    this->setStyleSheet("QPushButton{"
                            "color: green;"
                            "font-family: TimesNewRoman;"
                            "font-style: normal;"
                            "font-size: 12pt;"
                            "font-weight: bold;}"
                        "QLabel{"
                            "color: white;"
                            "font-family: TimesNewRoman;"
                            "font-style: normal;"
                            "font-size: 15pt;"
                            "font-weight: bold;}");

}

void OperationModeWidget::setupConnects(){

    connect(pButtonClockControlMode, SIGNAL(clicked(bool)), this, SLOT(slotShowClockControlModeWidget()));
    //connect(pButtonAutomaticMode, SIGNAL(clicked(bool)), this, SLOT(slotShowAutomaticModeWidget()));
    connect(pButtonControllerControlMode, SIGNAL(clicked(bool)), this, SLOT(slotShowControllerControlModeWidget()));

}

void OperationModeWidget::slotShowClockControlModeWidget(){

    emit showclockcontrollmodewidget();
}

void OperationModeWidget::slotShowAutomaticModeWidget(){

    emit showautomaticmodewidget();
}

void OperationModeWidget::slotShowControllerControlModeWidget(){

    emit showcontrollercontrolmodewidget();
}

OperationModeWidget::~OperationModeWidget(){

}
