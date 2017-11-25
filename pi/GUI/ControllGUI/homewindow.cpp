#include "homewindow.h"
#include "ui_homewindow.h"

HomeWindow::HomeWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::HomeWindow)
{
    ui->setupUi(this);

    generateStartLayout();
}

HomeWindow::~HomeWindow()
{
    delete ui;
}


bool HomeWindow::generateStartLayout(){

    vBox1 = new QVBoxLayout(ui->centralWidget);

    progressBar = new QProgressBar(ui->centralWidget);
    pButtonExit = new QPushButton(ui->centralWidget);
    pButtonStart = new QPushButton(ui->centralWidget);

    pButtonStart->setText("Starte StarCar");
    pButtonExit->setText("Beenden");

    vBox1->addWidget(progressBar);
    vBox1->addWidget(pButtonExit);
    vBox1->addWidget(pButtonStart);

    return true;

}
