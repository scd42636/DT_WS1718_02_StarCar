#include "alertwindow.h"
#include "ui_alertwindow.h"

AlertWindow::AlertWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AlertWindow)
{
    ui->setupUi(this);
}

AlertWindow::~AlertWindow()
{
    delete ui;
}

void AlertWindow::on_pButtonExit_clicked()
{
    this->close();
}
