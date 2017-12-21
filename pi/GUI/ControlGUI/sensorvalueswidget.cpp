#include "sensorvalueswidget.h"

//#define NOTPI

SensorValuesWidget::SensorValuesWidget(QWidget *parent, Alert *alertThread, QString pButtonGoBackText, IBC *IBCPointer) : QWidget(parent)
{
    this->alertThread = alertThread;
    this->pButtonGoBackText = pButtonGoBackText;
    this->IBCPointer = IBCPointer;

    generateLayout();
    setupConnects();
    generateStyle();

#ifdef Q_OS_LINUX

 /*   iUltraFront     = new Inbox(this->IBCPointer->getInbox(180));
    iUltraBack      = new Inbox(this->IBCPointer->getInbox(181));
    iCompass        = new Inbox(this->IBCPointer->getInbox(182));
    iAcceleration   = new Inbox(this->IBCPointer->getInbox(183));
    */
    test            = new Inbox(this->IBCPointer->getInbox(254));

  /*  packetUltrafront    = new Packet(180,2);
    packetUltraback     = new Packet(181,2);
    packetCompass       = new Packet(182,3);
    packetAcceleration  = new Packet(183,6);*/
    char buff[4] = "ddd";
    packetTest          = new Packet(254,4,(uint8_t * )buff);

#ifdef NOTPI
    QThread *thread = new QThread;
    threadLidar     = new ThreadLidar(alertThread);

    threadLidar->moveToThread(thread);

    connect(thread, SIGNAL(started()), threadLidar, SLOT(startProcess()));
    connect(threadLidar, SIGNAL(finished()), thread, SLOT(quit()));
    connect(threadLidar, SIGNAL(finished()), threadLidar, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();

    lidarTimer                = new QTimer();
    connect(lidarTimer, SIGNAL(timeout()), threadLidar, SLOT(finishLidar()),Qt::DirectConnection);
    lidarTimer->start(50000);
#endif

    QuerySensorValuesTimer = new QTimer();
    connect(QuerySensorValuesTimer, SIGNAL(timeout()), this, SLOT(slotQuerySensorValues()));
    QuerySensorValuesTimer->start(2000);

#endif

}

void SensorValuesWidget::generateLayout(){

    vBox1 = new QVBoxLayout(this);
    hBox1 = new QHBoxLayout();

    vBoxLabelDescription    = new QVBoxLayout();
    vBoxLabelValues         = new QVBoxLayout();

    lblUltraFront           = new QLabel();
    lblUltraBack            = new QLabel();
    lblcompass              = new QLabel();
    lblacceleration         = new QLabel();
    lblUWB                  = new QLabel();

    lblUltraFrontValue      = new QLabel();
    lblUltraBackValue       = new QLabel();
    lblcompassValue         = new QLabel();
    lblaccelerationValue    = new QLabel();
    lblUWBValue             = new QLabel();

    pButtonGoBack           = new QPushButton();

    vBox1->addLayout(hBox1);

    hBox1->addLayout(vBoxLabelDescription);
    hBox1->addLayout(vBoxLabelValues);

    vBoxLabelDescription->addWidget(lblUltraFront);
    vBoxLabelDescription->addWidget(lblUltraBack);
    vBoxLabelDescription->addWidget(lblcompass);
    vBoxLabelDescription->addWidget(lblacceleration);
    vBoxLabelDescription->addWidget(lblUWB);

    vBoxLabelValues->addWidget(lblUltraFrontValue);
    vBoxLabelValues->addWidget(lblUltraBackValue);
    vBoxLabelValues->addWidget(lblcompassValue);
    vBoxLabelValues->addWidget(lblaccelerationValue);
    vBoxLabelValues->addWidget(lblUWBValue);

    vBox1->addWidget(pButtonGoBack);

    lblValues.append(lblUltraFrontValue);
    lblValues.append(lblUltraBackValue);
    lblValues.append(lblcompassValue);
    lblValues.append(lblaccelerationValue);
    lblValues.append(lblUWBValue);

    for (QLabel *lables : lblValues){
        lables->setText("--");
    }
}

void SensorValuesWidget::setupConnects(){

    connect(pButtonGoBack, SIGNAL(clicked(bool)), this, SLOT(slotpButtonGoBackPushed()));
}

void SensorValuesWidget::generateStyle(){

    this->setStyleSheet("QLabel{"
                            "color: white;"
                            "font-family: TimesNewRoman;"
                            "font-style: normal;"
                            "font-size: 10pt;"
                            "font-weight: bold;}"
                        "QPushButton{"
                            "color: green;"
                            "font-family: TimesNewRoman;"
                            "font-style: normal;"
                            "font-size: 10pt;"
                            "font-weight: bold;}");

    vBox1->setContentsMargins(8,0,8,0);

    lblUltraFront->     setText("Ultraschall Vorne: ");
    lblUltraBack->      setText("Ultraschall Hinten: ");
    lblcompass->        setText("Kompass: ");
    lblacceleration->   setText("Beschleunigung: ");
    lblUWB->            setText("UWB: ");

    pButtonGoBack->setText(pButtonGoBackText);
}

void SensorValuesWidget::slotpButtonGoBackPushed(){

    //threadLidar->finishLidar();
    emit removeWindowfromStack();
}

void SensorValuesWidget::slotQuerySensorValues(){

    #ifdef Q_OS_LINUX

  /*  iUltraBack->fetch();
    iUltraFront->fetch();
    iCompass->fetch();
    iAcceleration->fetch();*/
    test->fetch();

    /*IBCPointer->send(*packetUltrafront);
    IBCPointer->send(*packetUltraback);
    IBCPointer->send(*packetCompass);
    IBCPointer->send(*packetAcceleration);*/
    IBCPointer->send(*packetTest);
/*
    lblUltraBackValue->setText(iUltraBack.front());
    lblUltraFrontValue->setText(iUltraFront.front());
    lblcompassValue->setText(iCompass.front());
    lblaccelerationValue->setText(iAcceleration.front());
*/

    if(test->empty()) std::cout << "empty\n";
    else
    {
        std::stringstream ss;
        ss << *(test->front());
        std::string s = ss.str();
        lblUltraBackValue->setText(s.c_str());
    }
    //*test->front()
    //lblUltraBackValue->setText();

    #endif

}
