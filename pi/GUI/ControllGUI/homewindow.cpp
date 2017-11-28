#include "homewindow.h"
#include "ui_homewindow.h"
#include "startwidget.h"



HomeWindow::HomeWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::HomeWindow)
{

    ui->setupUi(this);
    generateStartLayout();

}

void HomeWindow::generateStartLayout(){

    centralVBox = new QVBoxLayout(ui->centralWidget);
    mainStackedWidget = new QStackedWidget();

    centralVBox->addWidget(mainStackedWidget);

    QWidget *startwidget = new StartWidget(nullptr,this);

    addWidgetToMainStackWidget(startwidget);
    styleWindow();

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

}

void HomeWindow::addWidgetToMainStackWidget (QWidget *widget){

    mainStackedWidget->addWidget(widget);
    widget->setContentsMargins(0,0,0,0);
}

void HomeWindow::closeApp(){

    this->close();
}

HomeWindow::~HomeWindow()
{
    delete ui;
}
