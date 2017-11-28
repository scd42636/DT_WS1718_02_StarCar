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
    hBox1 = new QHBoxLayout();

    // Generate needed UI-Elements
    progressBar = new QProgressBar();
    pButtonExit = new QPushButton(QIcon("C:/Users/Flo/Desktop/DT_WS1718_02_StarCar/pi/GUI/ControllGUI/Pics/exit.png"),"");
    pButtonStart = new QPushButton(QIcon("C:/Users/Flo/Desktop/DT_WS1718_02_StarCar/pi/GUI/ControllGUI/Pics/start.png"),"");
    pButtonAlert = new QPushButton(QIcon("C:/Users/Flo/Desktop/DT_WS1718_02_StarCar/pi/GUI/ControllGUI/Pics/alert_green.png"),"");
    lblHeadline = new QLabel();

    // Add all UI-Elements to the layout
    vBox1->addWidget(lblHeadline,0,Qt::AlignHCenter);
    vBox1->addSpacing(20);
    vBox1->addWidget(pButtonStart,0,Qt::AlignHCenter);
    vBox1->addSpacing(20);
    vBox1->addWidget(progressBar);
    vBox1->addSpacing(40);
    vBox1->addLayout(hBox1);
    hBox1->addWidget(pButtonAlert);
    hBox1->addSpacing(200);
    hBox1->addWidget(pButtonExit);

}

void StartWidget::setupUIElements(){

    // progressBar
    progressBar->setRange(0,14);
    progressBar->setValue(0);
    progressBar->setTextVisible(false);
}

void StartWidget::setupConnects(){

    // connect exit button to close function -> once the exit-button is clicked the application will close
    connect(pButtonExit, SIGNAL(clicked()), MainWindow, SLOT(closeApp()));

    //connect the start-button clicked event with methode ...
    connect(pButtonStart, SIGNAL(clicked()), this, SLOT(fillProgressBar()));

    connect(pButtonAlert, SIGNAL(clicked(bool)), this , SLOT(showAlert()));
}

void StartWidget::styleWidget(){

/********************************Texts********************************************/

    lblHeadline->setText("StarCar");

    pButtonStart->setIconSize(QSize(32,32));
    pButtonStart->resize(32,32);

    pButtonExit->setIconSize(QSize(32,32));
    pButtonExit->resize(32,32);

    pButtonAlert->setIconSize(QSize(32,32));
    pButtonAlert->resize(32,32);

/******************************StyleSheets****************************************/

            this->setStyleSheet("QWidget{"
                                "background-color: #2b2b2b;}");

    lblHeadline->setStyleSheet("QLabel{"
                               "color: green;"
                               "font-family: TimesNewRoman;"
                               "font-style: normal;"
                               "font-size: 15pt;"
                               "font-weight: bold;}");

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

    pButtonExit->setStyleSheet("QPushButton{"
                               "border-radius:  10px;"
                               "border-width:   3px;"
                               "border-color:   black"
                               "boder-style:    solid}");

    pButtonAlert->setStyleSheet("QPushButton{"
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

    this->close();
}

void StartWidget::showAlert(){

}


StartWidget::~StartWidget(){

}
