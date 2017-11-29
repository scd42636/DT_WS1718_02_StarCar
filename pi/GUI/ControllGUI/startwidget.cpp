#include "startwidget.h"
#include "homewindow.h"
#include "alert.h"

StartWidget::StartWidget(QWidget *parent, QMainWindow *mainwindow) : QWidget(parent)
{
    MainWindow = mainwindow;

    generateStartLayout();
    setupUIElements();
    setupConnects();
    styleWidget();
}

void StartWidget::generateStartLayout(){

    //
    vBox1 = new QVBoxLayout(this);

    // Generate needed UI-Elements
    progressBar = new QProgressBar();
    pButtonStart = new QPushButton(QIcon("C:/Users/Flo/Desktop/DT_WS1718_02_StarCar/pi/GUI/ControllGUI/Pics/start.png"),"");

    // Add all UI-Elements to the layout
    vBox1->addWidget(pButtonStart,0,Qt::AlignHCenter);
    vBox1->addSpacing(20);
    vBox1->addWidget(progressBar);
    vBox1->addSpacing(40);
}

void StartWidget::setupUIElements(){

    // progressBar
    progressBar->setRange(0,14);
    progressBar->setValue(0);
    progressBar->setTextVisible(false);
}

void StartWidget::setupConnects(){

    //connect the start-button clicked event with methode ...
    connect(pButtonStart, SIGNAL(clicked()), this, SLOT(fillProgressBar()));
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

void StartWidget::fillProgressBar(){


    /*if(progressBar->value() == 100)
    {
        for(int i=100; i>=0; i-=20)
        {
            progressBar->setValue(i);
            this->thread()->sleep(1);

        }
        pButtonAlert->setIcon(QIcon("C:/Users/Flo/Desktop/DT_WS1718_02_StarCar/pi/GUI/ControllGUI/Pics/alert_white.png"));

    }else{

        for(int i=0; i<=100; i+=20)
        {
            progressBar->setValue(i);
            this->thread()->sleep(1);

        }
        pButtonAlert->setIcon(QIcon("C:/Users/Flo/Desktop/DT_WS1718_02_StarCar/pi/GUI/ControllGUI/Pics/alert_red.png"));
    }*/

    QThread *thread = new QThread;
    Alert  *worker = new Alert(pButtonAlert);
    worker->moveToThread(thread);
    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void StartWidget::closeStarCar(){

    //this->close();
    delete this;
}

void StartWidget::showAlert(){

}


StartWidget::~StartWidget(){

}
