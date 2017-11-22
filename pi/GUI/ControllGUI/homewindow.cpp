#include "homewindow.h"
#include "alertwindow.h"
#include "ui_homewindow.h"

HomeWindow::HomeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HomeWindow)
{
    ui->setupUi(this);
    /*QPixmap bkgnd("/Users/florianboemmel/DT_WS1718_02_StarCar/pi/GUI/ControllGUI/Pics/StarCar.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);*/
    //QObject::connect(quitButton, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));
}

HomeWindow::~HomeWindow()
{
    delete ui;
}

void HomeWindow::on_pButtonStart_clicked()
{
    this->setStyleSheet("background-color:green;");
}

void HomeWindow::on_pButtonStop_clicked()
{
    this->setStyleSheet("background-color:red;");
}

void HomeWindow::on_pButtonExit_clicked()
{
    this->close();
}

void HomeWindow::on_pButtonAlert_clicked()
{
    AlertWindow *alertWindow = new AlertWindow();
    alertWindow->show();
}
