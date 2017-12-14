#include "controllercontrolmodewidget.h"
#include <../../lidar/lidar.h>

ControllerControlModeWidget::ControllerControlModeWidget(QWidget *parent, Alert *alertThread, IBC *IBCPointer) : QWidget(parent)
{
    this->alertThread = alertThread;
    this->IBCPointer = IBCPointer;

    generateLayout();
    setupConnects();
    generateStyle();

    blinkTimer = new QTimer(this);
    connect(blinkTimer, SIGNAL(timeout()), this, SLOT(blinklblInfo()));
    blinkTimer->start(700);

    // LIDAR HIER
    // != 0 get mesure fehler
    // =0 passt alles

    lidar test;
    int rep = test.get_measurement("testfile.txt");
    if(rep == 0){
        qDebug("Lidar läuft");
    }

}

void ControllerControlModeWidget::generateLayout(){

    vBox1           = new QVBoxLayout(this);
    pButtonGoBack   = new QPushButton();
    pButtonNext     = new QPushButton();
    lblInfo         = new QLabel();

    vBox1->addWidget(lblInfo);
    vBox1->addSpacing(20);
    vBox1->addWidget(pButtonNext);
    vBox1->addWidget(pButtonGoBack);
}

void ControllerControlModeWidget::generateStyle(){

    vBox1->setContentsMargins(0,0,0,0);
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
}

void ControllerControlModeWidget::setupConnects(){

    connect(pButtonGoBack, SIGNAL(clicked(bool)), this, SLOT(pButtonGoBackPushed()));
    connect(pButtonNext, SIGNAL(clicked(bool)), this, SLOT(pButtonNextPushed()));
}

void ControllerControlModeWidget::pButtonNextPushed(){

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

    PortToArduino = new Serial("/dev/ttyUSB0");
    PortToArduino->send("1",1);
}

void ControllerControlModeWidget::createControllAnimation(){

    hBoxImages              = new QHBoxLayout();
    hBoxImages->setAlignment(Qt::AlignHCenter);

    vBoxRightTexts          = new QVBoxLayout();
    vBoxLeftTexts           = new QVBoxLayout();
    vBoxRightImagesArrow    = new QVBoxLayout();
    vBoxLeftImagesArrow     = new QVBoxLayout();

    hBoxArrowsLeft          = new QHBoxLayout();
    hBoxTextsLeft           = new QHBoxLayout();
    vBoxLeftImagesAndTexts  = new QVBoxLayout();
    pButtonNext             = new QPushButton();

    vBox1->insertLayout(1,hBoxImages);
    hBoxImages->setContentsMargins(0,10,0,0);

    lblTextBreak            = new QLabel("Bremse");
    setStyletoLabel(lblTextBreak, Qt::AlignLeft);

    lblTextSpeedUp          = new QLabel("Gas");
    setStyletoLabel(lblTextSpeedUp, Qt::AlignLeft);

    lblTextTurnLeft         = new QLabel("Links");
    setStyletoLabel(lblTextTurnLeft, Qt::AlignHCenter);

    lblTextTurnRight        = new QLabel("Rechts");
    setStyletoLabel(lblTextTurnRight, Qt::AlignHCenter);

    QPixmap controller = QPixmap("://Pics/controller.png");

    lblImageViewController  = new QLabel();
    lblImageViewController->setPixmap(controller);
    lblImageViewController->setScaledContents(true);


    hBoxImages->addLayout(vBoxLeftImagesAndTexts);
    vBoxLeftImagesAndTexts->addLayout(hBoxArrowsLeft);
    vBoxLeftImagesAndTexts->addLayout(hBoxTextsLeft);

    hBoxImages->addWidget(lblImageViewController);

    hBoxImages->addLayout(vBoxRightImagesArrow);
    hBoxImages->addLayout(vBoxRightTexts);

    lblArrowUp               = new QLabel();
    setArrowPicsToLabel(lblArrowUp, "arrowUp", 25, 50);

    lblArrowDown             = new QLabel();
    setArrowPicsToLabel(lblArrowDown, "arrowDown", 25, 50);

    lblArrowLeft             = new QLabel();
    setArrowPicsToLabel(lblArrowLeft, "arrowLeft", 40, 50);

    lblArrowRight            = new QLabel();
    setArrowPicsToLabel(lblArrowRight, "arrowRight", 40, 50);

    hBoxArrowsLeft->addWidget(lblArrowLeft);
    hBoxArrowsLeft->addWidget(lblArrowRight);

    vBoxRightImagesArrow->addWidget(lblArrowUp);
    vBoxRightImagesArrow->addWidget(lblArrowDown);

    vBoxRightTexts->addWidget(lblTextSpeedUp);
    vBoxRightTexts->addWidget(lblTextBreak);

    hBoxTextsLeft->addWidget(lblTextTurnLeft);
    hBoxTextsLeft->addWidget(lblTextTurnRight);

    hboxButtonsBottom->addWidget(pButtonNext);
    pButtonNext->setText("Zeige Sensorwerte");
    connect(pButtonNext, SIGNAL(clicked(bool)), this, SLOT(slotShowSensorValues()));

    vBox1->setContentsMargins(8,0,8,0);

    blinkTimer                = new QTimer();
    connect(blinkTimer, SIGNAL(timeout()), this, SLOT(blinkArrows()));
    blinkTimer->start(700);
}

void ControllerControlModeWidget::slotShowSensorValues(){

    delete PortToArduino;
    emit showsensorvalueswidget();
}

void ControllerControlModeWidget::pButtonGoBackPushed(){

    delete PortToArduino;
    emit removeWindowfromStack();
}

void ControllerControlModeWidget::blinklblInfo(){

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

void ControllerControlModeWidget::blinkArrows(){

    if(lblArrowLeft->isHidden()){

        lblArrowUp->show();
        lblArrowDown->show();
        lblArrowRight->show();
        lblArrowLeft->show();
        blinkTimer->setInterval(1500);


    }else{

        lblArrowLeft->hide();
        lblArrowUp->hide();
        lblArrowDown->hide();
        lblArrowRight->hide();
        blinkTimer->setInterval(200);
    }

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
