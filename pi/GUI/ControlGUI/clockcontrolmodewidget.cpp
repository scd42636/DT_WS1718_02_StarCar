#include "clockcontrolmodewidget.h"
#include <QImage>

#define IBCNOTWORKING

ClockControllModeWidget::ClockControllModeWidget(QWidget *parent, Alert *alertThread, IBC *IBCPointer, Serial *SerialPortArduino) : QWidget(parent)
{
    this->alertThread = alertThread;
    this->IBCPointer = IBCPointer;
    this->SerialPortArduino = SerialPortArduino;

    setupWidget();
}

ClockControllModeWidget::~ClockControllModeWidget(){

}

/********************************************************************
 *                         Private Methodes                         *
 *******************************************************************/

void ClockControllModeWidget::setupWidget(){

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

    vBox1->setContentsMargins(0,0,0,0);
    vBox1->setAlignment(Qt::AlignBottom);
    lblInfo->setAlignment(Qt::AlignHCenter);

    lblInfo->setText("Nutze die Uhren, um das StarCar manuell zu steuern! \nLege jetzt die Uhren an dein Handgelenk an.");
    lblInfo->setWordWrap(true);
    lblInfo->setStyleSheet("QLabel{"
                               "color: orange;"
                               "font-family: TimesNewRoman;"
                               "font-style: normal;"
                               "font-size: 10pt;}");

    pButtonGoBack->setText("Zurück zur Moduswahl");
    pButtonNext->setText("Sizt, passt, wackelt und hat Luft!");

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

void ClockControllModeWidget::createControllAnimation(){

    hBoxImages              = new QHBoxLayout();
    hBoxImages->setAlignment(Qt::AlignHCenter);

    vBoxLeftTexts           = new QVBoxLayout();
    vBoxRightTexts          = new QVBoxLayout();
    vBoxLeftImagesArrow     = new QVBoxLayout();
    vBoxRightImagesArrow    = new QVBoxLayout();

    vBox1->insertLayout(1,hBoxImages);
    hBoxImages->setContentsMargins(0,10,0,0);

    lblTextBreak            = new QLabel("Bremse");
    setStyletoLabel(lblTextBreak, Qt::AlignRight);

    lblTextSpeedUp          = new QLabel("Gas");
    setStyletoLabel(lblTextSpeedUp, Qt::AlignRight);

    lblTextTurnLeft         = new QLabel("Links");
    setStyletoLabel(lblTextTurnLeft, Qt::AlignLeft);

    lblTextTurnRight        = new QLabel("Rechts");
    setStyletoLabel(lblTextTurnRight, Qt::AlignLeft);

    QPixmap clock = QPixmap("://Pics/clock.png");

    lblImageViewClockLeft   = new QLabel();
    lblImageViewClockLeft->setPixmap(clock);
    lblImageViewClockLeft->setScaledContents(true);

    lblImageViewClockRight  = new QLabel();
    lblImageViewClockRight->setPixmap(clock);
    lblImageViewClockRight->setScaledContents(true);

    hBoxImages->addWidget(lblImageViewClockRight);
    hBoxImages->addLayout(vBoxRightImagesArrow);
    hBoxImages->addLayout(vBoxRightTexts);

    hBoxImages->addLayout(vBoxLeftTexts);
    hBoxImages->addLayout(vBoxLeftImagesArrow);
    hBoxImages->addWidget(lblImageViewClockLeft);

    lblArrowUpRight         = new QLabel();
    setArrowPicsToLabel(lblArrowUpRight, "upright");

    lblArrowDownRight       = new QLabel();
    setArrowPicsToLabel(lblArrowDownRight, "downright");

    lblArrowUpLeft          = new QLabel();
    setArrowPicsToLabel(lblArrowUpLeft, "upleft");

    lblArrowDownLeft        = new QLabel();
    setArrowPicsToLabel(lblArrowDownLeft, "downleft");

    vBoxRightImagesArrow->addWidget(lblArrowUpLeft);
    vBoxRightImagesArrow->addWidget(lblArrowDownLeft);

    vBoxLeftImagesArrow->addWidget(lblArrowUpRight);
    vBoxLeftImagesArrow->addWidget(lblArrowDownRight);

    vBoxLeftTexts->addWidget(lblTextSpeedUp);
    vBoxLeftTexts->addWidget(lblTextBreak);

    vBoxRightTexts->addWidget(lblTextTurnLeft);
    vBoxRightTexts->addWidget(lblTextTurnRight);

    vBox1->setContentsMargins(8,0,8,0);

    blinkTimer              = new QTimer();
    connect(blinkTimer, SIGNAL(timeout()), this, SLOT(slotBlinkArrows()));
    blinkTimer->start(700);
}

void ClockControllModeWidget::setArrowPicsToLabel(QLabel *lbl, QString path){

    lbl->setPixmap(QPixmap("://Pics/" + path +".png"));
    lbl->setFixedHeight(25);
    lbl->setFixedWidth(50);
    lbl->setScaledContents(true);
    QSizePolicy sp_retain = lbl->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    lbl->setSizePolicy(sp_retain);
}

void ClockControllModeWidget::setStyletoLabel(QLabel *lbl, Qt::Alignment align){

    lbl->setAlignment(align);
    lbl->setStyleSheet("QLabel{color: orange;font-family: TimesNewRoman;font-style: normal;font-size: 9pt;font-weight: bold;}");
}

/********************************************************************
 *                         Private SLOTS                            *
 *******************************************************************/

void ClockControllModeWidget::slotpButtonNextPushed(){

    lblInfo->setText("StarCar läuft mit der Uhrsteuerung!");
    lblInfo->setFixedHeight(20);

    QString stylesheetString = "QLabel{color: green;font-family: TimesNewRoman;font-style: normal;font-size: 12pt;}";
    lblInfo->setStyleSheet(stylesheetString);
    vBox1->removeWidget(pButtonNext);
    vBox1->removeWidget(pButtonGoBack);

    delete pButtonNext;
    pButtonNextRemoved = true;

    hBoxButtonsBottom = new QHBoxLayout();
    vBox1->addLayout(hBoxButtonsBottom);
    hBoxButtonsBottom->addWidget(pButtonGoBack);

    pButtonNext             = new QPushButton();
    hBoxButtonsBottom->addWidget(pButtonNext);
    pButtonNext->setText("Zeige Sensorwerte");
    connect(pButtonNext, SIGNAL(clicked(bool)), this, SLOT(slotShowSensorValues()));

    blinkTimer->stop();
    delete blinkTimer;

    createControllAnimation();

#ifdef Q_OS_LINUX

    #ifdef IBCNOTWORKING

        PortToArduino->send("2",1);
    #endif

#endif

}

void ClockControllModeWidget::slotpButtonGoBackPushed(){

    emit removeWindowformStack();
}

void ClockControllModeWidget::slotShowSensorValues(){

    emit showsensorvalueswidget();
}

void ClockControllModeWidget::slotBlinklblInfo(){

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

void ClockControllModeWidget::slotBlinkArrows(){

    if(lblArrowUpLeft->isHidden()){

        lblArrowUpLeft->show();
        lblArrowDownLeft->show();
        lblArrowUpRight->show();
        lblArrowDownRight->show();
        blinkTimer->setInterval(1500);


    }else{

        lblArrowDownLeft->hide();
        lblArrowUpLeft->hide();
        lblArrowDownRight->hide();
        lblArrowUpRight->hide();
        blinkTimer->setInterval(200);
    }
}
