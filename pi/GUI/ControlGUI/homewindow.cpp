#include "homewindow.h"
#include "ui_homewindow.h"

HomeWindow::HomeWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::HomeWindow)
{
    ui->setupUi(this);
    generateLayout();
    generateStyle();
    setupConnects();
    createAlertThread();
    slotShowStartWidget();
}

void HomeWindow::generateLayout(){

    centralVBox         = new QVBoxLayout(ui->centralWidget);
    hBox1               = new QHBoxLayout();
    lblHeadline         = new QLabel();
    pButtonAlert        = new QPushButton(QIcon("://Pics/alert_white.png"),"");
    pButtonExit         = new QPushButton(QIcon("://Pics/exit.png"),"");

    mainStackedWidget   = new QStackedWidget();

    centralVBox->addSpacing(8);
    centralVBox->addWidget(lblHeadline,0,Qt::AlignHCenter);
    centralVBox->addSpacing(12);
    centralVBox->addWidget(mainStackedWidget);
    centralVBox->addLayout(hBox1);

    hBox1->addSpacing(5);
    hBox1->addWidget(pButtonAlert);
    hBox1->addSpacing(200);
    hBox1->addWidget(pButtonExit);
    hBox1->addSpacing(5);

    centralVBox->addSpacing(5);
}

void HomeWindow::setupConnects(){

    connect(pButtonExit, SIGNAL(clicked(bool)), this, SLOT(slotShowExitWidget()));
    connect(pButtonAlert, SIGNAL(clicked(bool)), this , SLOT(slotShowAlertWidget()));
}

void HomeWindow::createAlertThread(){

    QThread *thread = new QThread;
    alertThread     = new Alert(pButtonAlert);
    alertThread->moveToThread(thread);

    connect(thread, SIGNAL(started()), alertThread, SLOT(process()));
    connect(alertThread, SIGNAL(finished()), thread, SLOT(quit()));
    connect(alertThread, SIGNAL(finished()), alertThread, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();

    alertTimer      = new QTimer(this);
    connect(alertTimer, SIGNAL(timeout()), alertThread, SLOT(changeAlertIcon()));
    alertTimer->start(300);
}

void HomeWindow::generateStyle(){

/********************************Margins******************************************/

    ui->centralWidget->setContentsMargins(0,0,0,0);
    centralVBox->setContentsMargins(0,0,0,0);

/******************************StyleSheets****************************************/

    this->setStyleSheet("QWidget{"
                        "background-color: #2b2b2b;}");
    //#2b2b2b

/*****************************Windowstyle*****************************************/

    this->setFixedSize(320,240);
    setWindowFlags(Qt::FramelessWindowHint);

/*****************************Button & Label**************************************/

    pButtonExit->setIconSize(QSize(32,32));
    pButtonExit->resize(32,32);

    pButtonAlert->setIconSize(QSize(32,32));
    pButtonAlert->resize(32,32);

    lblHeadline->setText("StarCar");

    lblHeadline->setStyleSheet("QLabel{"
                               "color: yellow;"
                               "font-family: TimesNewRoman;"
                               "font-style: normal;"
                               "font-size: 15pt;"
                               "font-weight: bold;}");

    pButtonExit->setStyleSheet("QPushButton{"
                               "border-radius:  10px;"
                               "border-width:   3px;"
                               "border-color:   black"
                               "border-style:    solid}");

    pButtonAlert->setStyleSheet("QPushButton{"
                                "border-radius:  10px;"
                                "border-width:   5px;"
                                "border-color:   black"
                                "border-style:    solid}");
}

void HomeWindow::addWidgetToMainStackWidget (QWidget *widget){

    mainStackedWidget->addWidget(widget);
    widget->setContentsMargins(0,0,0,0);
    mainStackedWidget->setCurrentWidget(widget);
}

void HomeWindow::removeActiveWidget(){

    mainStackedWidget->removeWidget(mainStackedWidget->currentWidget());
}

void HomeWindow::slotShowSensorValuesWidget(){

#ifndef IBCNOTWORKING
    sensorValuesWidget = new SensorValuesWidget(this, this->alertThread, "Zurück zur Moduswahl" ,this-> IBCPointer);
#else
     sensorValuesWidget = new SensorValuesWidget(this->msg, this, this->alertThread, "Zurück zur Moduswahl", this->protocol, this->serialPort);
#endif

    connect(sensorValuesWidget, SIGNAL(removeWindowfromStack()), this, SLOT(removeActiveWidget()));
    addWidgetToMainStackWidget(sensorValuesWidget);
}

void HomeWindow::slotShowSensorValuesWidgetAfterControlMode(){

#ifndef IBCNOTWORKING
    sensorValuesWidget = new SensorValuesWidget(this, this->alertThread, "Zurück zur Steuerung",  this->IBCPointer);
#else
     sensorValuesWidget = new SensorValuesWidget(this->msg, this, this->alertThread, "Zurück zur Steuerung", this->protocol, this->serialPort);
#endif

    connect(sensorValuesWidget, SIGNAL(removeWindowfromStack()), this, SLOT(removeActiveWidget()));
    addWidgetToMainStackWidget(sensorValuesWidget);
}

void HomeWindow::slotShowClockControlModeWidget(){

#ifndef IBCNOTWORKING
    clockcontrolModeWidget = new ClockControllModeWidget(this, this->alertThread, this->IBCPointer);
#else
    clockcontrolModeWidget = new ClockControllModeWidget(this->msg, this, this->alertThread, this->protocol);
#endif

    connect(clockcontrolModeWidget, SIGNAL(removeWindowformStack()), this, SLOT(removeActiveWidget()));
    connect(clockcontrolModeWidget, SIGNAL(showsensorvalueswidget()), this, SLOT(slotShowSensorValuesWidgetAfterControlMode()));
    addWidgetToMainStackWidget(clockcontrolModeWidget);
}

void HomeWindow::slotShowControllerControlModeWidget(){

#ifndef IBCNOTWORKING
    controllercontrolModeWidget = new ControllerControlModeWidget(this, this->alertThread, this->IBCPointer);
#else
    controllercontrolModeWidget = new ControllerControlModeWidget(this->msg, this, this->alertThread, this->protocol);
#endif


    connect(controllercontrolModeWidget, SIGNAL(removeWindowfromStack()), this, SLOT(removeActiveWidget()));
    connect(controllercontrolModeWidget, SIGNAL(showsensorvalueswidget()), this, SLOT(slotShowSensorValuesWidgetAfterControlMode()));
    addWidgetToMainStackWidget(controllercontrolModeWidget);
}

void HomeWindow::slotShowOperationModeWidget(){

    IBCactive = true;
    operationModeWidget = new OperationModeWidget(this, this->alertThread);
    connect(operationModeWidget, SIGNAL(removeWindowformStack()), this, SLOT(removeActiveWidget()));
    connect(operationModeWidget, SIGNAL(showclockcontrollmodewidget()), this, SLOT(slotShowClockControlModeWidget()));
    connect(operationModeWidget, SIGNAL(showcontrollercontrolmodewidget()), this, SLOT(slotShowControllerControlModeWidget()));
    connect(operationModeWidget, SIGNAL(showsensorvalueswidget()), this, SLOT(slotShowSensorValuesWidget()));
    addWidgetToMainStackWidget(operationModeWidget);
}

void HomeWindow::slotShowExitWidget(){

#ifndef IBCNOTWORKING
    exitWidget = new ExitWidget(this, this->alertThread, &this->IBCPointer, IBCactive);
#else
    exitWidget = new ExitWidget(this, this->alertThread, &this->serialPort, IBCactive);
#endif

    connect(exitWidget, SIGNAL(removeWindowformStack()), this, SLOT(removeActiveWidget()));
    addWidgetToMainStackWidget(exitWidget);
}

void HomeWindow::slotShowAlertWidget(){

    if(alertWidget == nullptr){

        alertWidget = new AlertWidget(this, this->alertThread);
        connect(alertWidget, SIGNAL(removeWindowformStack()), this, SLOT(removeActiveWidget()));
        addWidgetToMainStackWidget(alertWidget);

    }else{

        removeActiveWidget();
        alertWidget = nullptr;
    }
}

void HomeWindow::slotShowStartWidget(){

#ifndef IBCNOTWORKING
    startWidget = new StartWidget(nullptr, this->alertThread, &this->IBCPointer);
#else
    startWidget = new StartWidget(&this->msg, nullptr, this->alertThread, &this->serialPort, &this->protocol);
#endif
    connect(startWidget,SIGNAL(showOperationMode()),SLOT(slotShowOperationModeWidget()));
    addWidgetToMainStackWidget(startWidget);
}

HomeWindow::~HomeWindow()
{
    delete ui;
}
