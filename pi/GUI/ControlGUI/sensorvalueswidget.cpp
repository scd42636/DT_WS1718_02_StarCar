#include "sensorvalueswidget.h"

#define PI

SensorValuesWidget::SensorValuesWidget(QWidget *parent, Alert *alertThread, QString pButtonGoBackText, StarCarProtocol *starcarprotocol) : QWidget(parent)
{
    this->alertThread = alertThread;
    this->pButtonGoBackText = pButtonGoBackText;
    this->starcarprotocol = starcarprotocol;

    generateLayout();
    setupConnects();
    generateStyle();

    QDir SensorOutputFolder("/home/pi/SensorOutput");
    SensorOutputFolder.setFilter(QDir::NoDotAndDotDot | QDir::Files);
    foreach (QString SensorOutputItem, SensorOutputFolder.entryList()){
        SensorOutputFolder.remove(SensorOutputItem);
    }

#ifdef Q_OS_LINUX

#ifdef PI

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
    //lidarTimer->start(500);

#endif

    QuerySensorValuesTimer = new QTimer();
    connect(QuerySensorValuesTimer, SIGNAL(timeout()), this, SLOT(slotQuerySensorValues()));
    QuerySensorValuesTimer->start(500);

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
#ifdef Q_OS_LINUX

    threadLidar->finishLidar();
    QuerySensorValuesTimer->stop();
    emit removeWindowfromStack();

#endif
}

void SensorValuesWidget::slotQuerySensorValues(){

#ifdef Q_OS_LINUX

    starcarprotocol->setRequest(CarSensorRequest_All);
    starcarprotocol->send();
    starcarprotocol->receive();

    lblUltraFrontValue->setText(QString::number((int)starcarprotocol->getDistanceFront()));
    lblUltraBackValue->setText(QString::number((int)starcarprotocol->getDistanceBack()));
    lblcompassValue->setText(QString::number((int)starcarprotocol->getCompass()));
    lblaccelerationValue->setText("X: " + QString::number((int)starcarprotocol->getAccelerationX()) +
                                      " Y: " + QString::number((int)starcarprotocol->getAccelerationY()));


#endif
}


QString SensorValuesWidget::getMesureValue(){

}





