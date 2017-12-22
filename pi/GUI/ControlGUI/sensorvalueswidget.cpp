#include "sensorvalueswidget.h"

#define PI
//#define IBCTEST

SensorValuesWidget::SensorValuesWidget(QWidget *parent, Alert *alertThread, QString pButtonGoBackText, IBC *IBCPointer) : QWidget(parent)
{
    this->alertThread = alertThread;
    this->pButtonGoBackText = pButtonGoBackText;
    this->IBCPointer = IBCPointer;

    generateLayout();
    setupConnects();
    generateStyle();

    QDir SensorOutputFolder("/home/pi/SensorOutput");
    SensorOutputFolder.setFilter(QDir::NoDotAndDotDot | QDir::Files);
    foreach (QString SensorOutputItem, SensorOutputFolder.entryList()){
        SensorOutputFolder.remove(SensorOutputItem);
    }

#ifdef Q_OS_LINUX

    #ifdef IBCTEST
        test            = new Inbox(this->IBCPointer->getInbox(254));
        char buff[4] = "ddd";
        packetTest          = new Packet(254,4,(uint8_t * )buff);

    #else

        iUltraFront     = new Inbox(this->IBCPointer->getInbox(180));
        iUltraBack      = new Inbox(this->IBCPointer->getInbox(181));
        iCompass        = new Inbox(this->IBCPointer->getInbox(182));
        iAcceleration   = new Inbox(this->IBCPointer->getInbox(183));
        iUWB            = new Inbox(this->IBCPointer->getInbox(184));


        packetUltrafront    = new Packet(180,2);
        packetUltraback     = new Packet(181,2);
        packetCompass       = new Packet(182,3);
        packetAcceleration  = new Packet(183,6);
        packetUWB           = new Packet(184,6);
    #endif


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
    lidarTimer->start(50000);

#endif

    QuerySensorValuesTimer = new QTimer();
    connect(QuerySensorValuesTimer, SIGNAL(timeout()), this, SLOT(slotQuerySensorValues()));
    QuerySensorValuesTimer->start(1000);

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

    threadLidar->finishLidar();
    QuerySensorValuesTimer->stop();
    emit removeWindowfromStack();
}

void SensorValuesWidget::slotQuerySensorValues(){

#ifdef Q_OS_LINUX

    #ifdef IBCTEST
        test->fetch();
        IBCPointer->send(*packetTest);
        test->empty()          ? alertThread->fireWarning("Test kein Wert!") : lblUltraFrontValue->setText(getMesureValue(test));
    #else
        iUltraBack->fetch();
        iUltraFront->fetch();
        iCompass->fetch();
        iAcceleration->fetch();

        IBCPointer->send(*packetUltrafront);
        IBCPointer->send(*packetUltraback);
        IBCPointer->send(*packetCompass);
        IBCPointer->send(*packetAcceleration);

        iUltraFront->empty()   ? alertThread->fireWarning("Ultraschall vorne kein Wert!") : lblUltraFrontValue->setText(getMesureValue(iUltraFront));
        iUltraBack->empty()    ? alertThread->fireWarning("Ultraschall hinten kein Wert!") : lblUltraBackValue->setText(getMesureValue(iUltraBack));
        iCompass->empty()      ? alertThread->fireWarning("Kompass kein Wert!") : lblcompassValue->setText(getMesureValue(iCompass));
        iAcceleration->empty() ? alertThread->fireWarning("Beschleunigung kein Wert!") : lblaccelerationValue->setText(getMesureValue(iAcceleration));
        iUWB->empty()          ? alertThread->fireWarning("UWB kein Wert!") : lblUWBValue->setText(getMesureValue(iUWB));

    #endif

#endif
}


QString SensorValuesWidget::getMesureValue(Inbox *inbox){

    Packet tempPacket = *inbox->back();
    uint8_t *packetContent = tempPacket.content();
    int ID = (int)tempPacket.id();
    char *packetContentChar = (char *)packetContent;
    packetContentChar[(int)tempPacket.contentsize()] = '\0';

    QString resultValue;

    for (int i = 0; i < (int)tempPacket.contentsize(); i++){

        resultValue += QString::number(packetContentChar[i]);
    }

    QString filePath;

    switch (ID){

        case 180:
        {
            filePath = "/home/pi/SensorOutput/UltraVorne.txt";
            break;
        }

        case 181:
        {
            filePath = "/home/pi/SensorOutput/UltraHinten.txt";
            break;
        }

        case 182:
        {
            filePath = "/home/pi/SensorOutput/Compass.txt";
            break;
        }

        case 183:
        {
            filePath = "/home/pi/SensorOutput/Beschleunigung.txt";
            break;
        }

        case 184:
        {
            filePath = "/home/pi/SensorOutput/UWB.txt";
            break;
        }

        case 254:
        {
            filePath = "/home/pi/SensorOutput/test.txt";
            break;
        }

        default:
        {
            break;
        }

    }

    QFile file(filePath);

    if(file.open(QIODevice::WriteOnly | QIODevice::Append)){

        QTextStream stream (&file);
        stream << resultValue << endl;
        file.close();

    }else{

        alertThread->fireError("Could not open File" + filePath);
    }

    inbox->pop_front();
    return resultValue;
}





