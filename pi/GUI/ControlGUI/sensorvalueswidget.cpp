#include "sensorvalueswidget.h"

SensorValuesWidget::SensorValuesWidget(QWidget *parent, Alert *alertThread, QString pButtonGoBackText, IBC *IBCPointer, Serial *SerialPortArduino) : QWidget(parent)
{
    this->alertThread = alertThread;
    this->pButtonGoBackText = pButtonGoBackText;
    this->IBCPointer = IBCPointer;
    this->SerialPortArduino = SerialPortArduino;

    generateLayout();
    setupConnects();
    generateStyle();

#ifdef Q_OS_LINUX
/*
    testInbox = new Inbox(this->IBCPointer->getInbox());
    delete testInbox;
    testInbox = new Inbox(this->IBCPointer->getInbox(180));

    Inbox iUltraFront = this->IBCPointer->getInbox(180);
    Inbox iUltraBack = this->IBCPointer->getInbox(181);
    Inbox iCompass = this->IBCPointer->getInbox(182);
    Inbox iAcceleration = this->IBCPointer->getInbox(183);
*/

    iUltraFront     = new Inbox(this->IBCPointer->getInbox(180));
    iUltraBack      = new Inbox(this->IBCPointer->getInbox(181));
    iCompass        = new Inbox(this->IBCPointer->getInbox(182));
    iAcceleration   = new Inbox(this->IBCPointer->getInbox(183));

    packetUltrafront    = new Packet(180,2);
    packetUltraback     = new Packet(181,2);
    packetCompass       = new Packet(182,3);
    packetAcceleration  = new Packet(183,6);

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
    lblLidar                = new QLabel();

    lblUltraFrontValue      = new QLabel();
    lblUltraBackValue       = new QLabel();
    lblcompassValue         = new QLabel();
    lblaccelerationValue    = new QLabel();
    lblUWBValue             = new QLabel();
    lblLidarValue           = new QLabel();

    pButtonGoBack           = new QPushButton();

    vBox1->addLayout(hBox1);

    hBox1->addLayout(vBoxLabelDescription);
    hBox1->addLayout(vBoxLabelValues);

    vBoxLabelDescription->addWidget(lblUltraFront);
    vBoxLabelDescription->addWidget(lblUltraBack);
    vBoxLabelDescription->addWidget(lblcompass);
    vBoxLabelDescription->addWidget(lblacceleration);
    vBoxLabelDescription->addWidget(lblUWB);
    vBoxLabelDescription->addWidget(lblLidar);

    vBoxLabelValues->addWidget(lblUltraFrontValue);
    vBoxLabelValues->addWidget(lblUltraBackValue);
    vBoxLabelValues->addWidget(lblcompassValue);
    vBoxLabelValues->addWidget(lblaccelerationValue);
    vBoxLabelValues->addWidget(lblUWBValue);
    vBoxLabelValues->addWidget(lblLidarValue);

    vBox1->addWidget(pButtonGoBack);

    lblValues.append(lblUltraFrontValue);
    lblValues.append(lblUltraBackValue);
    lblValues.append(lblcompassValue);
    lblValues.append(lblaccelerationValue);
    lblValues.append(lblUWBValue);
    lblValues.append(lblLidarValue);

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
                            "font-size: 8pt;"
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
    lblLidar->          setText("Lidar");

    pButtonGoBack->setText(pButtonGoBackText);
}

void SensorValuesWidget::slotpButtonGoBackPushed(){

    emit removeWindowfromStack();
}

void SensorValuesWidget::slotQuerySensorValues(){

    #ifdef Q_OS_LINUX

    iUltraBack->fetch();
    iUltraFront->fetch();
    iCompass->fetch();
    iAcceleration->fetch();

    IBCPointer->send(*packetUltrafront);
    IBCPointer->send(*packetUltraback);
    IBCPointer->send(*packetCompass);
    IBCPointer->send(*packetAcceleration);
/*
    lblUltraBackValue->setText(iUltraBack.front());
    lblUltraFrontValue->setText(iUltraFront.front());
    lblcompassValue->setText(iCompass.front());
    lblaccelerationValue->setText(iAcceleration.front());
*/
    #endif

}
