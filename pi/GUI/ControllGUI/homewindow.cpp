#include "homewindow.h"
#include "ui_homewindow.h"
#include "startwidget.h"



HomeWindow::HomeWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::HomeWindow)
{

    ui->setupUi(this);
    generateStartLayout();
    styleWindow();
    setupConnects();

}

void HomeWindow::generateStartLayout(){

    centralVBox         = new QVBoxLayout(ui->centralWidget);
    hBox1               = new QHBoxLayout();
    lblHeadline         = new QLabel();
    pButtonAlert        = new QPushButton(QIcon("C:/Users/Flo/Desktop/DT_WS1718_02_StarCar/pi/GUI/ControllGUI/Pics/alert_green.png"),"");
    pButtonExit         = new QPushButton(QIcon("C:/Users/Flo/Desktop/DT_WS1718_02_StarCar/pi/GUI/ControllGUI/Pics/exit.png"),"");

    mainStackedWidget   = new QStackedWidget();

    centralVBox->addWidget(lblHeadline,0,Qt::AlignHCenter);
    centralVBox->addSpacing(20);
    centralVBox->addWidget(mainStackedWidget);
    centralVBox->addLayout(hBox1);

    hBox1->addSpacing(5);
    hBox1->addWidget(pButtonAlert);
    hBox1->addSpacing(200);
    hBox1->addWidget(pButtonExit);
    hBox1->addSpacing(5);

    centralVBox->addSpacing(5);

    startWidget = new StartWidget(nullptr,this);
    QWidget *testwidget = new StartWidget();

    addWidgetToMainStackWidget(startWidget);
    //addWidgetToMainStackWidget(testwidget);
}

void HomeWindow::setupConnects(){

    // connect exit button to close function -> once the exit-button is clicked the application will close
    connect(pButtonExit, SIGNAL(clicked(bool)), this, SLOT(removeActiveWidget()));
    connect(pButtonAlert, SIGNAL(clicked(bool)), this , SLOT(showAlert()));
}

void HomeWindow::styleWindow(){

/********************************Margins******************************************/

    ui->centralWidget->setContentsMargins(0,0,0,0);
    centralVBox->setContentsMargins(0,0,0,0);

/******************************StyleSheets****************************************/

    this->setStyleSheet("QWidget{"
                        "background-color: #2b2b2b;}");

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
                               "color: green;"
                               "font-family: TimesNewRoman;"
                               "font-style: normal;"
                               "font-size: 15pt;"
                               "font-weight: bold;}");

    pButtonExit->setStyleSheet("QPushButton{"
                               "border-radius:  10px;"
                               "border-width:   3px;"
                               "border-color:   black"
                               "boder-style:    solid}");

    pButtonAlert->setStyleSheet("QPushButton{"
                                "border-radius:  10px;"
                                "border-width:   5px;"
                                "border-color:   black"
                                "boder-style:    solid}");
}

void HomeWindow::addWidgetToMainStackWidget (QWidget *widget){

    mainStackedWidget->addWidget(widget);
    widget->setContentsMargins(0,0,0,0);
}

void HomeWindow::removeActiveWidget(){

    mainStackedWidget->removeWidget(startWidget);
    //delete startWidget;
}

HomeWindow::~HomeWindow()
{
    delete ui;
}
