#include "controllercontrolmodewidget.h"

//#define IBCNOTWORKING

ControllerControlModeWidget::ControllerControlModeWidget(QWidget *parent, Alert *alertThread, IBC *IBCPointer) : QWidget(parent)
{
    this->alertThread = alertThread;
    this->IBCPointer = IBCPointer;

    setupWidget();
}

ControllerControlModeWidget::~ControllerControlModeWidget(){

}

/********************************************************************
 *                         Private Methodes                         *
 *******************************************************************/

void ControllerControlModeWidget::setupWidget(){

    // Generate all needed UI-Elements

    vBox1           = new QVBoxLayout(this);
    pButtonGoBack   = new QPushButton();
    pButtonNext     = new QPushButton();
    lblInfo         = new QLabel();

    // Add UI-Elements to Vertical Box Layout

    vBox1->addWidget(lblInfo);
    vBox1->addSpacing(20);
    vBox1->addWidget(pButtonNext);
    vBox1->addWidget(pButtonGoBack);

    // Set Style-Properties to the UI-Elements

    vBox1->setContentsMargins(8,0,8,0);
    vBox1->setAlignment(Qt::AlignBottom);
    lblInfo->setAlignment(Qt::AlignHCenter);

    lblInfo->setText("Nutze den Controller, um das StarCar manuell \n zu steuern!");
    lblInfo->setWordWrap(true);

    lblInfo->setStyleSheet("QLabel{"
                               "color: orange;"
                               "font-family: TimesNewRoman;"
                               "font-style: normal;"
                               "font-size: 10pt;}");

    pButtonGoBack->setText("Zurück zur Moduswahl");
    pButtonNext->setText("Controller griffbereit!");

    this->setStyleSheet("QPushButton{"
                            "color: green;"
                            "font-family: TimesNewRoman;"
                            "font-style: normal;"
                            "font-size: 10pt;"
                            "font-weight: bold;}");

    // Connect actions like button pushed with the slot which will execute after

    connect(pButtonGoBack, SIGNAL(clicked(bool)), this, SLOT(slotpButtonGoBackPushed()));
    connect(pButtonNext, SIGNAL(clicked(bool)), this, SLOT(slotpButtonNextPushed()));

    // Setup a timer with period 700ms and connect the timeout action to slotBlinklblInfo

    blinkTimer = new QTimer(this);
    connect(blinkTimer, SIGNAL(timeout()), this, SLOT(slotBlinklblInfo()));
    blinkTimer->start(700);
}

void ControllerControlModeWidget::createControllAnimation(){

    // Generate all needed UI-Elements

    hBoxImages              = new QHBoxLayout();
    vBoxLeftControl         = new QVBoxLayout();
    vBoxRightControl        = new QVBoxLayout();

    hBoxLeftArrows          = new QHBoxLayout();
    hBoxLeftTexts           = new QHBoxLayout();
    hBoxRightArrowsAndTexts = new QHBoxLayout();
    hBoxController          = new QHBoxLayout();

    vBox1->insertLayout(1,hBoxImages);
    vBox1->removeWidget(lblInfo);
    delete lblInfo;

    hBoxImages->addLayout(vBoxLeftControl);
    hBoxImages->addLayout(vBoxRightControl);

    lblInfo                 = new QLabel();
    lblInfo->setText("Controllersteuerung");

    QString stylesheetString = "QLabel{color: green;font-family: TimesNewRoman;font-style: bold;font-size: 11pt;}";
    lblInfo->setStyleSheet(stylesheetString);

    vBoxLeftControl->addWidget(lblInfo);
    vBoxLeftControl->addLayout(hBoxLeftArrows);
    vBoxLeftControl->addLayout(hBoxLeftTexts);

    lblTextTurnLeft         = new QLabel("Links");
    setStyletoLabel(lblTextTurnLeft, Qt::AlignHCenter);

    lblTextTurnRight        = new QLabel("Rechts");
    setStyletoLabel(lblTextTurnRight, Qt::AlignHCenter);

    lblArrowLeft             = new QLabel();
    setArrowPicsToLabel(lblArrowLeft, "arrowLeft", 40, 50);

    lblArrowRight            = new QLabel();
    setArrowPicsToLabel(lblArrowRight, "arrowRight", 40, 50);

    hBoxLeftArrows->addWidget(lblArrowLeft);
    hBoxLeftArrows->addWidget(lblArrowRight);

    hBoxLeftTexts->addWidget(lblTextTurnLeft);
    hBoxLeftTexts->addWidget(lblTextTurnRight);

    vBoxRightControl->addLayout(hBoxRightArrowsAndTexts);

    QPixmap controller = QPixmap("://Pics/controller.png");

    lblImageViewController  = new QLabel();
    lblImageViewController->setPixmap(controller);
    lblImageViewController->setScaledContents(true);
    lblImageViewController->setFixedSize(100,50);

    vBoxRightControl->addLayout(hBoxController);
    hBoxController->addSpacing(40);
    hBoxController->addWidget(lblImageViewController);

    lblTextSpeedUp          = new QLabel("Gas");
    setStyletoLabel(lblTextSpeedUp, Qt::AlignLeft);

    lblTextBreak            = new QLabel("Bremse");
    setStyletoLabel(lblTextBreak, Qt::AlignRight);

    lblArrowDown            = new QLabel();
    setArrowPicsToLabel(lblArrowDown, "arrowDown", 25, 50);

    lblArrowDown2           = new QLabel();
    setArrowPicsToLabel(lblArrowDown2, "arrowDown", 25, 50);

    hBoxRightArrowsAndTexts->addSpacing(5);
    hBoxRightArrowsAndTexts->addWidget(lblTextBreak);
    hBoxRightArrowsAndTexts->addWidget(lblArrowDown);
    hBoxRightArrowsAndTexts->addWidget(lblArrowDown2);
    hBoxRightArrowsAndTexts->addWidget(lblTextSpeedUp);

    pButtonNext               = new QPushButton();
    hboxButtonsBottom->addWidget(pButtonNext);
    pButtonNext->setText("Zeige Sensorwerte");
    connect(pButtonNext, SIGNAL(clicked(bool)), this, SLOT(slotShowSensorValues()));

    blinkTimer                = new QTimer();
    connect(blinkTimer, SIGNAL(timeout()), this, SLOT(slotBlinkArrows()));
    blinkTimer->start(700);
}

void ControllerControlModeWidget::setArrowPicsToLabel(QLabel *lbl, QString path, int height, int width){

    lbl->setPixmap(QPixmap("://Pics/" + path +".png"));
    lbl->setFixedHeight(height);//25
    lbl->setFixedWidth(width);//50
    lbl->setScaledContents(true);
    QSizePolicy sp_retain = lbl->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    lbl->setSizePolicy(sp_retain);
}

void ControllerControlModeWidget::setStyletoLabel(QLabel *lbl, Qt::Alignment align){

    lbl->setAlignment(align);
    lbl->setStyleSheet("QLabel{color: orange;font-family: TimesNewRoman;font-style: normal;font-size: 9pt;font-weight: bold;}");
}

/********************************************************************
 *                         Private SLOTS                            *
 *******************************************************************/

void ControllerControlModeWidget::slotpButtonNextPushed(){

    lblInfo->setText("Controllersteuerung");
    lblInfo->setFixedHeight(20);

    QString stylesheetString = "QLabel{color: green;font-family: TimesNewRoman;font-style: normal;font-size: 12pt;}";
    lblInfo->setStyleSheet(stylesheetString);

    vBox1->removeWidget(pButtonNext);
    vBox1->removeWidget(pButtonGoBack);

    delete pButtonNext;
    pButtonNextRemoved = true;

    hboxButtonsBottom = new QHBoxLayout();

    vBox1->addLayout(hboxButtonsBottom);
    hboxButtonsBottom->addWidget(pButtonGoBack);

    blinkTimer->stop();
    delete blinkTimer;

    createControllAnimation();
}

void ControllerControlModeWidget::slotShowSensorValues(){

    emit showsensorvalueswidget();
}

void ControllerControlModeWidget::slotpButtonGoBackPushed(){

    emit removeWindowfromStack();
}

void ControllerControlModeWidget::slotBlinklblInfo(){

    QString stylesheetString;
    QString fontColor;

    if(pButtonNextRemoved){

        fontColor = "green";

    }else{

        fontColor = "orange";
    }

    if(fontSize == 10){

        stylesheetString = "QLabel{color: " + fontColor + ";font-family: TimesNewRoman;font-style: normal;font-size: 10pt;}";
        fontSize = 11;
    }else{

        stylesheetString = "QLabel{color: " + fontColor + ";font-family: TimesNewRoman;font-style: normal;font-size: 11pt;}";
        fontSize = 10;
    }

   lblInfo->setStyleSheet(stylesheetString);
}

void ControllerControlModeWidget::slotBlinkArrows(){

    if(lblArrowLeft->isHidden()){

        lblArrowDown2->show();
        lblArrowDown->show();
        lblArrowRight->show();
        lblArrowLeft->show();

        if(lastTimeWasShort){

            blinkTimer->setInterval(1500);
            lastTimeWasShort = false;

        }else{

            blinkTimer->setInterval(200);
            lastTimeWasShort = true;
        }


    }else{

        lblArrowLeft->hide();
        lblArrowDown2->hide();
        lblArrowDown->hide();
        lblArrowRight->hide();
        blinkTimer->setInterval(200);
    }

}
