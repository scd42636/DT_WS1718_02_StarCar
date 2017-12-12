#include "manualmodewidget.h"
#include <QImage>

ManualModeWidget::ManualModeWidget(QWidget *parent, Alert *alertThread) : QWidget(parent)
{
    this->alertThread = alertThread;

    generateLayout();
    setupConnects();
    generateStyle();

    /*QThread *thread = new QThread;
    manualMode= new ManualMode(alertThread);
    manualMode->moveToThread(thread);

    connect(thread, SIGNAL(started()), manualMode, SLOT(startProcess()));
    connect(manualMode, SIGNAL(finished()), thread, SLOT(quit()));
    connect(manualMode, SIGNAL(finished()), manualMode, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
*/
    blinkTimer = new QTimer(this);
    connect(blinkTimer, SIGNAL(timeout()), this, SLOT(blinklblInfo()));
    blinkTimer->start(700);

}

void ManualModeWidget::generateLayout(){

    vBox1           = new QVBoxLayout(this);
    pButtonGoBack   = new QPushButton();
    pButtonNext     = new QPushButton();
    lblInfo         = new QLabel();

    vBox1->addWidget(lblInfo);
    vBox1->addSpacing(20);
    vBox1->addWidget(pButtonNext);
    vBox1->addWidget(pButtonGoBack);
}

void ManualModeWidget::generateStyle(){

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
                            "font-size: 12pt;"
                            "font-weight: bold;}");
}

void ManualModeWidget::blinklblInfo(){

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

void ManualModeWidget::setupConnects(){

    connect(pButtonGoBack, SIGNAL(clicked(bool)), this, SLOT(pButtonGoBackPushed()));
    connect(pButtonNext, SIGNAL(clicked(bool)), this, SLOT(pButtonNextPushed()));
}

void ManualModeWidget::pButtonGoBackPushed(){

    emit removeWindowformStack();
}

void ManualModeWidget::pButtonNextPushed(){

    lblInfo->setText("StarCar läuft nun im manuellen Modus!");
    lblInfo->setFixedHeight(20);

    QString stylesheetString = "QLabel{color: green;font-family: TimesNewRoman;font-style: normal;font-size: 12pt;}";
    lblInfo->setStyleSheet(stylesheetString);
    vBox1->removeWidget(pButtonNext);
    delete pButtonNext;
    pButtonNextRemoved = true;

    blinkTimer->stop();
    delete blinkTimer;

    createControllAnimation();
}

void ManualModeWidget::createControllAnimation(){

    hBoxImages = new QHBoxLayout();
    hBoxImages->setAlignment(Qt::AlignHCenter);

    vBoxLeftTexts = new QVBoxLayout();
    vBoxRightTexts = new QVBoxLayout();
    vBoxLeftImagesArrow = new QVBoxLayout();
    vBoxRightImagesArrow = new QVBoxLayout();

    vBox1->insertLayout(1,hBoxImages);



    lblTextBreak = new QLabel("Bremse");
    setStyletoLabel(lblTextBreak, Qt::AlignRight);

    lblTextSpeedUp = new QLabel("Gas");
    setStyletoLabel(lblTextSpeedUp, Qt::AlignRight);

    lblTextTurnLeft = new QLabel("Links");
    setStyletoLabel(lblTextTurnLeft, Qt::AlignLeft);

    lblTextTurnRight = new QLabel("Rechts");
    setStyletoLabel(lblTextTurnRight, Qt::AlignLeft);

    lblImageViewClockLeft = new QLabel();
    lblImageViewClockLeft->setPixmap(QPixmap("://Pics/clock.png"));

    lblImageViewClockRight = new QLabel();
    lblImageViewClockRight->setPixmap(QPixmap("://Pics/clock.png"));
    lblImageViewClockRight->setFixedHeight(50);
    lblImageViewClockRight->setFixedWidth(50);
    lblImageViewClockRight->setScaledContents(true);

    hBoxImages->addWidget(lblImageViewClockRight);
    hBoxImages->addLayout(vBoxRightImagesArrow);
    hBoxImages->addLayout(vBoxRightTexts);

    hBoxImages->addLayout(vBoxLeftTexts);
    hBoxImages->addLayout(vBoxLeftImagesArrow);
    hBoxImages->addWidget(lblImageViewClockLeft);

    lblArrowUpRight = new QLabel();
    setArrowPicsToLabel(lblArrowUpRight, "upright");

    lblArrowDownRight = new QLabel();
    setArrowPicsToLabel(lblArrowDownRight, "downright");

    lblArrowUpLeft = new QLabel();
    setArrowPicsToLabel(lblArrowUpLeft, "upleft");

    lblArrowDownLeft = new QLabel();
    setArrowPicsToLabel(lblArrowDownLeft, "downleft");

    vBoxRightImagesArrow->addWidget(lblArrowUpLeft);
    vBoxRightImagesArrow->addWidget(lblArrowDownLeft);

    vBoxLeftImagesArrow->addWidget(lblArrowUpRight);
    vBoxLeftImagesArrow->addWidget(lblArrowDownRight);

    vBoxLeftTexts->addWidget(lblTextSpeedUp);
    vBoxLeftTexts->addWidget(lblTextBreak);

    vBoxRightTexts->addWidget(lblTextTurnLeft);
    vBoxRightTexts->addWidget(lblTextTurnRight);

    vBox1->setContentsMargins(0,0,0,0);

    blinkTimer = new QTimer();
    connect(blinkTimer, SIGNAL(timeout()), this, SLOT(blinkArrows()));
    blinkTimer->start(700);
}

void ManualModeWidget::blinkArrows(){

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

void ManualModeWidget::setArrowPicsToLabel(QLabel *lbl, QString path){

    lbl->setPixmap(QPixmap("://Pics/" + path +".png"));
    lbl->setFixedHeight(25);
    lbl->setFixedWidth(50);
    lbl->setScaledContents(true);
    QSizePolicy sp_retain = lbl->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    lbl->setSizePolicy(sp_retain);
}

void ManualModeWidget::setStyletoLabel(QLabel *lbl, Qt::Alignment align){

    lbl->setAlignment(align);
    lbl->setStyleSheet("QLabel{color: orange;font-family: TimesNewRoman;font-style: normal;font-size: 10pt;font-weight: bold;}");
}

ManualModeWidget::~ManualModeWidget(){

}
