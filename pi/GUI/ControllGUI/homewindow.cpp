#include "homewindow.h"
#include "ui_homewindow.h"

HomeWindow::HomeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HomeWindow)
{
    ui->setupUi(this);
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
