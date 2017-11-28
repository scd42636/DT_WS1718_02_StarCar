#include "homewindow.h"
#include "ui_homewindow.h"



HomeWindow::HomeWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::HomeWindow)
{

    ui->setupUi(this);
    this->setFixedSize(320,240);
    generateStartLayout();
    styleStartLayout();
}

HomeWindow::~HomeWindow()
{
    delete ui;
}


void HomeWindow::generateStartLayout(){

    // Set space between centralWidget and form to 0
    ui->centralWidget->setContentsMargins(0,0,0,0);

    centralVBox = new QVBoxLayout(ui->centralWidget);
    centralVBox->setContentsMargins(0,0,0,0);

    mainStackedWidget = new QStackedWidget();
    centralVBox->addWidget(mainStackedWidget);
    startWidget = new QWidget();

    addWidgetToMainStackWidget(startWidget);
    generateGUIElementsforStartWidget();

}

void HomeWindow::addWidgetToMainStackWidget (QWidget *widget){

    mainStackedWidget->addWidget(widget);
    widget->setContentsMargins(0,0,0,0);
}

void HomeWindow::generateGUIElementsforStartWidget(){

    // Add new verticalBox to central Widget
    vBox1 = new QVBoxLayout(startWidget);

    hBox1 = new QHBoxLayout();

    // Generate needed UI-Elements
    progressBar = new QProgressBar(ui->centralWidget);
    pButtonExit = new QPushButton(ui->centralWidget);
    pButtonStart = new QPushButton(ui->centralWidget);
    pButtonAlert = new QPushButton(ui->centralWidget);
    lblHeadline = new QLabel(ui->centralWidget);

    // Setup progressBar
    progressBar->setRange(0,100);
    progressBar->setValue(100);
    progressBar->setTextVisible(false);

    // Add all UI-Elements to the layout
    vBox1->addWidget(lblHeadline);
    vBox1->addSpacing(50);
    vBox1->addWidget(pButtonStart);
    vBox1->addWidget(progressBar);
    vBox1->addSpacing(50);
    vBox1->addItem(hBox1);

    hBox1->addWidget(pButtonAlert);
    hBox1->addWidget(pButtonExit);


    // connect exit button to close function -> once the exit-button is clicked the application will close
    connect(pButtonExit, SIGNAL(clicked()), this, SLOT(closeStarCar()));

    //connect the start-button clicked event with methode ...
    connect(pButtonStart, SIGNAL(clicked()), this, SLOT(fillProgressBar()));

    connect(pButtonAlert, SIGNAL(clicked(bool)), this , SLOT(showAlert()));
}

void HomeWindow::styleStartLayout(){

    ui->centralWidget->setStyleSheet("QWidget{"
                                     "background-color: #2b2b2b;}");

    vBox1->setAlignment(Qt::AlignTop);

    lblHeadline->setText("StarCar");

    lblHeadline->setStyleSheet("QLabel{"
                               "color: green;"
                               "font-family: TimesNewRoman;"
                               "font-style: normal;"
                               "font-size: 15pt;"
                               "font-weight: bold;}");

    //lblHeadline->setFixedHeight(20);
    lblHeadline->setAlignment(Qt::AlignCenter);

    progressBar->setStyleSheet("QProgressBar{"
                               "background-color: black;"
                               "border: 2px solid grey;"
                               "border-radius: 5px;}"

                               "QProgressBar::chunk{"
                               "background-color: #08ff08;"
                               "width: 10px;"
                               "margin: 5px;}");

    pButtonStart->setText("Starte StarCar");
    pButtonStart->setStyleSheet("QPushButton{"
                                "background-color: green;}");

    pButtonExit->setText("Beenden");
    pButtonExit->setStyleSheet("QPushButton{"
                                "background-color: green;}");

    pButtonAlert->setText("ALARM!!");

    // Hide default window hints
    setWindowFlags(Qt::FramelessWindowHint);

}

void HomeWindow::fillProgressBar(){


    if(progressBar->value() == 100)
    {
        for(int i=100; i>=0; i-=20)
        {
            progressBar->setValue(i);
            this->thread()->sleep(1);

        }
    }else{

        for(int i=0; i<=100; i+=20)
        {
            progressBar->setValue(i);
            this->thread()->sleep(1);

        }
    }
}

void HomeWindow::closeStarCar(){

    this->close();
}

void HomeWindow::showAlert(){

}

