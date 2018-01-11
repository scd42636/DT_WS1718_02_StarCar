#include "sensorvalueswidget.h"

#ifndef IBCNOTWORKING

typedef uint8_t InoByte_t;
typedef int16_t InoInt_t;

typedef struct StarCarSonicData_t
{
    InoInt_t Value;

} StarCarSonicData;

typedef struct StarCarMagnetData_t
{
    InoByte_t Parity;
    InoInt_t Value;

} StarCarMagnetData;

typedef struct StarCarAccelerationData_t
{
    InoByte_t ParityOfXValue;
    InoInt_t  XValue;
    InoByte_t ParityOfYValue;
    InoInt_t  YValue;

} StarCarAccelerationData;
#endif

SensorValuesWidget::SensorValuesWidget(QWidget *parent, Alert *alertThread, QString pButtonGoBackText, IBC *IBCPointer) : QWidget(parent)
{
    this->alertThread = alertThread;
    this->pButtonGoBackText = pButtonGoBackText;
    this->IBCPointer = IBCPointer;

    generateLayout();
    setupConnects();
    generateStyle();
}

SensorValuesWidget::SensorValuesWidget(message *msg, QWidget *parent, Alert *alertThread, QString pButtonGoBackText
                                       , StreamSerialProtocol *protocol) : QWidget(parent)
{
    this->alertThread = alertThread;
    this->pButtonGoBackText = pButtonGoBackText;
    this->protocol = protocol;
    this->msg = msg;

    generateLayout();
    setupConnects();
    generateStyle();
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

    QDir SensorOutputFolder("/home/pi/SensorOutput");
    SensorOutputFolder.setFilter(QDir::NoDotAndDotDot | QDir::Files);
    foreach (QString SensorOutputItem, SensorOutputFolder.entryList()){
        SensorOutputFolder.remove(SensorOutputItem);
    }

#ifdef Q_OS_LINUX
/*
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
*/

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

    /*    iUltraBack->fetch();
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
*/

    uint8_t receiveState;

    this->msg->Request = 1;
    this->msg->Mode = 2;
    protocol->send();
     protocol->send();
      protocol->send();

    //receiveState =  protocol->receive();


    if(receiveState == ProtocolState::SUCCESS ){

        lblUltraFrontValue->setText(QString::number((int)msg->DistanceFront));
        lblUltraBackValue->setText(QString::number((int)msg->DistanceBack));
        lblcompassValue->setText(QString::number((int)msg->DirectionValue));
        lblaccelerationValue->setText("X: " + QString::number((int)msg->AccelerationXValue)
                                      + "Y: " + QString::number((int)msg->AccelerationYValue));
    }



#endif
}

QString SensorValuesWidget::getMesureValue(Inbox *inbox){

#ifdef Q_OS_LINUX
/*
    QString resultValue;

    Packet tempPacket = *inbox->back();

    int ID = (int)tempPacket.id();

    QString filePath;

    switch (ID){

        case 180:
        {
            filePath = "/home/pi/SensorOutput/UltraVorne.txt";

            StarCarSonicData sonicFrontData;
            ReadData<StarCarSonicData>(&sonicFrontData, tempPacket);

            resultValue = QString::number((int)sonicFrontData.Value);
            break;
        }

        case 181:
        {
            filePath = "/home/pi/SensorOutput/UltraHinten.txt";

            StarCarSonicData sonicBackData;
            ReadData<StarCarSonicData>(&sonicBackData, tempPacket);

            resultValue = QString::number((int)sonicBackData.Value);
            break;
        }

        case 182:
        {
            filePath = "/home/pi/SensorOutput/Compass.txt";

            StarCarMagnetData magnetData;
            ReadData<StarCarMagnetData>(&magnetData, tempPacket);

            int value = (int)magnetData.Value;

            if (magnetData.Parity == 1)
                value *= -1;

            resultValue = QString::number(value);
            break;
        }

        case 183:
        {
            filePath = "/home/pi/SensorOutput/Beschleunigung.txt";

            StarCarAccelerationData acceleratorData;
            ReadData<StarCarAccelerationData>(&acceleratorData, tempPacket);

            int xValue = (int)acceleratorData.XValue;

            if (acceleratorData.ParityOfXValue == 1)
                xValue *= -1;

            resultValue = "X= " + QString::number(xValue);

            int yValue = (int)acceleratorData.YValue;

            if (acceleratorData.ParityOfYValue == 1)
                yValue *= -1;

            resultValue += ", Y= " + QString::number(yValue);
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
*/
#endif

}





