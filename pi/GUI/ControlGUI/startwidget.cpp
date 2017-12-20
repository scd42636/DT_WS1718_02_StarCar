#include "startwidget.h"
#include "homewindow.h"
#include "alert.h"
#include "initstarcar.h"

bool progressfull = false;
bool progressBarTimerisNOTRunning = true;

StartWidget::StartWidget(QWidget *parent, Alert *alertThread, IBC **IBCPointer, Serial **SerialPortArduino) : QWidget(parent)
{
    generateLayout();
    setupProgressBar();
    setupConnects();
    generateStyle();

    this->alertThread = alertThread;
    this->IBCPointer = IBCPointer;
    this->SerialPortArduino = SerialPortArduino;
}

void StartWidget::generateLayout(){

    vBox1           = new QVBoxLayout(this);
    progressBar     = new QProgressBar();
    pButtonStart    = new QPushButton(QIcon("://Pics/start.png"),"");

    vBox1->addWidget(pButtonStart);
    vBox1->addSpacing(20);
    vBox1->addWidget(progressBar);
    vBox1->addSpacing(40);
}

void StartWidget::setupProgressBar(){

    progressBar->setRange(0,14);
    progressBar->setValue(0);
    progressBar->setTextVisible(false);
}

void StartWidget::setupConnects(){

    connect(pButtonStart, SIGNAL(clicked()), this, SLOT(initializeStarCar()));
}

void StartWidget::generateStyle(){

/******************************Button-Size****************************************/

    pButtonStart->setIconSize(QSize(32,32));
    pButtonStart->resize(40,40);

/******************************StyleSheet*****************************************/

            this->setStyleSheet("QWidget{"
                                    "background-color: #2b2b2b;}"
                                "QProgressBar{"
                                    "background-color: black;"
                                    "border: 2px solid grey;"
                                    "border-radius: 5px;}"
                                "QProgressBar::chunk{"
                                    "background-color: #08ff08;"
                                    "width: 10px;"
                                    "margin: 5px;}"
                                "QPushButton{"
                                    "border-radius:  0px;"
                                    "border-width:   0px;"
                                    "border-color:   white;"
                                    "border-style:   solid;}");

/*****************************Windowstyle*****************************************/

    vBox1->setAlignment(Qt::AlignHCenter);
}

// Initialize function will be run, once the start button is pushed

void StartWidget::initializeStarCar(){

    pButtonStart->setEnabled(false);

    QThread *thread = new QThread;
    initStarCar     = new InitStarCar(alertThread, IBCPointer, SerialPortArduino);
    initStarCar->moveToThread(thread);

    connect(thread, SIGNAL(started()), initStarCar, SLOT(startProcess()));
    connect(initStarCar, SIGNAL(finished()), thread, SLOT(quit()));
    connect(initStarCar, SIGNAL(finished()), initStarCar, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    connect(initStarCar, SIGNAL(pushProcessBar()), SLOT(fillProgressBar()));

    thread->start();
}

void StartWidget::fillProgressBar(){

    if(progressfull){

        emit showOperationMode();

    }else{

        progressBar->setValue(progressBar->value() + 1);
    }

    if(progressBar->value() == 14)
    {
        progressfull = true;
    }
}

StartWidget::~StartWidget(){

}
