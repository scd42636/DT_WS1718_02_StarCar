#include "homewindow.h"
#include "ui_homewindow.h"

HomeWindow::HomeWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::HomeWindow)
{
    ui->setupUi(this);
    generateLayout();
    generateStyle();
    setupConnects();
    createAlertThread();
    showStartWidget();
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

    connect(pButtonExit, SIGNAL(clicked(bool)), this, SLOT(showExitWidget()));
    connect(pButtonAlert, SIGNAL(clicked(bool)), this , SLOT(showAlertWidget()));
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

void HomeWindow::showSensorValuesWidget(){

    sensorValuesWidget = new SensorValuesWidget(this, this->alertThread);
    connect(sensorValuesWidget, SIGNAL(removeWindowfromStack()), this, SLOT(removeActiveWidget()));
    addWidgetToMainStackWidget(sensorValuesWidget);
}

void HomeWindow::showSensorValuesWidgetAfterControlMode(){

    sensorValuesWidget = new SensorValuesWidget(this, this->alertThread, "Zurück zur Steuerung");
    connect(sensorValuesWidget, SIGNAL(removeWindowfromStack()), this, SLOT(removeActiveWidget()));
    addWidgetToMainStackWidget(sensorValuesWidget);
}

void HomeWindow::showClockControlModeWidget(){

    clockcontrolModeWidget = new ClockControllModeWidget(this, this->alertThread);
    connect(clockcontrolModeWidget, SIGNAL(removeWindowformStack()), this, SLOT(removeActiveWidget()));
    addWidgetToMainStackWidget(clockcontrolModeWidget);
}

void HomeWindow::showControllerControlModeWidget(){

    controllercontrolModeWidget = new ControllerControlModeWidget(this, this->alertThread);
    connect(controllercontrolModeWidget, SIGNAL(removeWindowfromStack()), this, SLOT(removeActiveWidget()));
    connect(controllercontrolModeWidget, SIGNAL(showsensorvalueswidget()), this, SLOT(showSensorValuesWidgetAfterControlMode()));
    addWidgetToMainStackWidget(controllercontrolModeWidget);
}

void HomeWindow::showOperationModeWidget(){

    operationModeWidget = new OperationModeWidget(this, this->alertThread);
    connect(operationModeWidget, SIGNAL(removeWindowformStack()), this, SLOT(removeActiveWidget()));
    connect(operationModeWidget, SIGNAL(showclockcontrollmodewidget()), this, SLOT(showClockControlModeWidget()));
    connect(operationModeWidget, SIGNAL(showcontrollercontrolmodewidget()), this, SLOT(showControllerControlModeWidget()));
    connect(operationModeWidget, SIGNAL(showsensorvalueswidget()), this, SLOT(showSensorValuesWidget()));
    addWidgetToMainStackWidget(operationModeWidget);
}

void HomeWindow::showExitWidget(){

    exitWidget = new ExitWidget(this, this->alertThread);
    connect(exitWidget, SIGNAL(removeWindowformStack()), this, SLOT(removeActiveWidget()));
    addWidgetToMainStackWidget(exitWidget);
}

void HomeWindow::showAlertWidget(){

    if(alertWidget == nullptr){

        alertWidget = new AlertWidget(this, this->alertThread);
        connect(alertWidget, SIGNAL(removeWindowformStack()), this, SLOT(removeActiveWidget()));
        addWidgetToMainStackWidget(alertWidget);

    }else{

        removeActiveWidget();
        alertWidget = nullptr;
    }
}

void HomeWindow::showStartWidget(){

    startWidget = new StartWidget(nullptr, this->alertThread);
    connect(startWidget,SIGNAL(showOperationMode()),SLOT(showOperationModeWidget()));
    addWidgetToMainStackWidget(startWidget);
}

HomeWindow::~HomeWindow()
{
    delete ui;
}
