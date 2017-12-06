#include "startwidget.h"
#include "homewindow.h"
#include "alert.h"
#include "pathsandconstans.h"

bool progressfull = false;
bool progressBarTimerisNOTRunning = true;

StartWidget::StartWidget(QWidget *parent, Alert *alertThread) : QWidget(parent)
{
    generateLayout();
    setupProgressBar();
    setupConnects();
    generateStyle();

    progressBarTimer = new QTimer(this);
    connect(progressBarTimer, SIGNAL(timeout()), this, SLOT(fillProgressBar()));
    this->alertThread = alertThread;
}

void StartWidget::generateLayout(){

    // Generate needed UI-Elements
    vBox1 = new QVBoxLayout(this);
    progressBar = new QProgressBar();
    pButtonStart = new QPushButton(QIcon(startImage),"");

    // Add all UI-Elements to the layout
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

    //connect the start-button clicked event with methode ...
    connect(pButtonStart, SIGNAL(clicked()), this, SLOT(startProgressBar()));
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

void StartWidget::startProgressBar(){

    emit showOperationMode();
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

StartWidget::~StartWidget(){

}
