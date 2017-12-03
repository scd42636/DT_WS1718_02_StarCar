#include "startwidget.h"
#include "homewindow.h"
#include "alert.h"
#include "pathsandconstans.h"

bool progressfull = false;
bool progressBarTimerisNOTRunning = true;

StartWidget::StartWidget(QWidget *parent, Alert *alertThread) : QWidget(parent)
{
    generateStartLayout();
    setupUIElements();
    setupConnects();
    styleWidget();
    progressBarTimer = new QTimer(this);
    connect(progressBarTimer, SIGNAL(timeout()), this, SLOT(fillProgressBar()));
    this->alertThread = alertThread;
}

void StartWidget::generateStartLayout(){

    //
    vBox1 = new QVBoxLayout(this);

    // Generate needed UI-Elements
    progressBar = new QProgressBar();
    pButtonStart = new QPushButton(QIcon(startImage),"");

    // Add all UI-Elements to the layout
    vBox1->addWidget(pButtonStart,0,Qt::AlignHCenter);
    vBox1->addSpacing(20);
    vBox1->addWidget(progressBar);
    vBox1->addSpacing(40);
}

void StartWidget::setupUIElements(){

    // progressBar
    progressBar->setRange(0,14);
    progressBar->setValue(10);
    progressBar->setTextVisible(false);
}

void StartWidget::setupConnects(){

    //connect the start-button clicked event with methode ...
    connect(pButtonStart, SIGNAL(clicked()), this, SLOT(startProgressBar()));
}

void StartWidget::styleWidget(){

/********************************Texts********************************************/

    pButtonStart->setIconSize(QSize(32,32));
    pButtonStart->resize(32,32);

/******************************StyleSheets****************************************/

            this->setStyleSheet("QWidget{"
                                "background-color: #2b2b2b;}");

    progressBar->setStyleSheet("QProgressBar{"
                               "background-color: black;"
                               "border: 2px solid grey;"
                               "border-radius: 5px;}"

                               "QProgressBar::chunk{"
                               "background-color: #08ff08;"
                               "width: 10px;"
                               "margin: 5px;}");

    pButtonStart->setStyleSheet("QPushButton{"
                                "border-radius:  10px;"
                                "border-width:   3px;"
                                "border-color:   black"
                                "boder-style:    solid}");

/*****************************Windowstyle*****************************************/

    vBox1->setAlignment(Qt::AlignTop);
    //lblHeadline->setAlignment(Qt::AlignCenter);

    // Hide default window hints
    setWindowFlags(Qt::FramelessWindowHint);
}

void StartWidget::startProgressBar(){

    emit showOperationMode();

    /*alertThread->clearError();
    alertThread->clearWarning();

    if(progressBarTimerisNOTRunning){

        progressBarTimer->start(500);
        progressBarTimerisNOTRunning = false;

    }else{

        progressBarTimer->stop();
        progressBarTimerisNOTRunning = true;
    }*/
}

void StartWidget::fillProgressBar(){

    if(progressfull){
        progressBar->setValue(progressBar->value() - 1);
    }else{
        progressBar->setValue(progressBar->value() + 1);
    }

    if(progressBar->value() == 14){
        progressfull = true;
        alertThread->fireError();
    }
    if(progressBar->value() == 0){
        progressfull = false;
        alertThread->fireWarning();
    }

}

void StartWidget::closeStarCar(){

    delete this;
}

void StartWidget::showAlert(){

}


StartWidget::~StartWidget(){

}
