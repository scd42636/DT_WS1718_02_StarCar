#include "operationmodewidget.h"

OperationModeWidget::OperationModeWidget(QWidget *parent, Alert *alertThread) : QWidget(parent)
{
    this->alertThread = alertThread;
    generateLayout();
    generateStyle();
    setupConnects();
}

void OperationModeWidget::generateLayout(){

    vBox1                        = new QVBoxLayout(this);

    lblHeadline                  = new QLabel();

    pButtonClockControlMode      = new QPushButton();
    pButtonControllerControlMode = new QPushButton();
    pButtonSensorValues          = new QPushButton();

    vBox1->addWidget(lblHeadline);
    vBox1->addSpacing(10);

    vBox1->addWidget(pButtonClockControlMode);
    vBox1->addWidget(pButtonControllerControlMode);
    vBox1->addWidget(pButtonSensorValues);
}

void OperationModeWidget::generateStyle(){

    vBox1->setAlignment(Qt::AlignVCenter);

    lblHeadline->setAlignment(Qt::AlignHCenter);
    lblHeadline->setText("Betriebsmodi Auswahl");


    pButtonClockControlMode->setText("Uhrsteuerung");
    pButtonControllerControlMode->setText("Controllersteuerung");
    pButtonSensorValues->setText("Sensorwerte");

    pButtonClockControlMode->setMinimumHeight(30);
    pButtonControllerControlMode->setMinimumHeight(30);
    pButtonSensorValues->setMinimumHeight(30);

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
                            "font-size: 13pt;"
                            "font-weight: bold;}");

}

void OperationModeWidget::setupConnects(){

    connect(pButtonClockControlMode, SIGNAL(clicked(bool)), this, SLOT(slotShowClockControlModeWidget()));
    connect(pButtonControllerControlMode, SIGNAL(clicked(bool)), this, SLOT(slotShowControllerControlModeWidget()));
    connect(pButtonSensorValues, SIGNAL(clicked(bool)), this, SLOT(slotShowSensorValuesWidget()));

}

void OperationModeWidget::slotShowClockControlModeWidget(){

    emit showclockcontrollmodewidget();
}

void OperationModeWidget::slotShowControllerControlModeWidget(){

    emit showcontrollercontrolmodewidget();
}

void OperationModeWidget::slotShowSensorValuesWidget(){

    emit showsensorvalueswidget();
}

OperationModeWidget::~OperationModeWidget(){

}
