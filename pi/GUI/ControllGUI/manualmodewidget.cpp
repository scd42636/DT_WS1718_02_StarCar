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
    connect(blinkTimer, SIGNAL(timeout()), this, SLOT(blinkLable()));
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

    pButtonGoBack->setText("Zurück zur Moduswahl");
    pButtonNext->setText("Sizt, passt, wackelt und hat Luft!");

    this->setStyleSheet("QLabel{"
                        "color: orange;"
                        "font-family: TimesNewRoman;"
                        "font-style: normal;"
                        "font-size: 12pt;}"
                        "QPushButton{"
                        "color: green;"
                        "font-family: TimesNewRoman;"
                        "font-style: normal;"
                        "font-size: 12pt;"
                        "font-weight: bold;}");
}

void ManualModeWidget::blinkLable(){

    QString stylesheetString;

    if(fontSize < 13){

        stylesheetString = "QLabel{color: orange;font-family: TimesNewRoman;font-style: normal;font-size: " + QString::number(fontSize) + "pt;}";
        fontSize+= 0.5;
    }else{
        fontSize = 12.5;
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

    QString stylesheetString = "QLabel{color: green;font-family: TimesNewRoman;font-style: normal;font-size: 12pt;}";
    lblInfo->setStyleSheet(stylesheetString);
    vBox1->removeWidget(pButtonNext);
    delete pButtonNext;

    hBoxImages = new QHBoxLayout();

    hBoxImages->setAlignment(Qt::AlignHCenter);

    lblImageleft = new QLabel();
    lblImageleft->setPixmap(QPixmap("://Pics/clock.png"));

    lblImageright = new QLabel();
    lblImageright->setPixmap(QPixmap("://Pics/clock.png"));

    lblDown = new QLabel("Zurück");
    lblUp = new QLabel("Vor");

    vBoxleftImage = new QVBoxLayout();
    vBoxrightImage = new QVBoxLayout();

    vBox1->insertLayout(1,hBoxImages);

    hBoxImages->addLayout(vBoxleftImage);
    hBoxImages->addWidget(lblImageleft);
    hBoxImages->addWidget(lblImageright);
    hBoxImages->addLayout(vBoxrightImage);

    vBoxleftImage->addWidget(lblUp);
    vBoxleftImage->addWidget(lblDown);

    lblLeft = new QLabel("Links");
    lblright = new QLabel("Rechts");

    vBoxrightImage->addWidget(lblLeft);
    vBoxrightImage->addWidget(lblright);

    vBox1->setContentsMargins(0,0,0,0);
}

ManualModeWidget::~ManualModeWidget(){

}
