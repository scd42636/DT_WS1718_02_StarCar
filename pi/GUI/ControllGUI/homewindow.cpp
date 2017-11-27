#include "homewindow.h"
#include "ui_homewindow.h"
#include <QTimer>
#include <QThread>


HomeWindow::HomeWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::HomeWindow)
{
    ui->setupUi(this);
    this->setFixedSize(320,240);
    generateStartLayout();
}

HomeWindow::~HomeWindow()
{
    delete ui;
}


bool HomeWindow::generateStartLayout(){

    // Set space between centralWidget and form to 8
    ui->centralWidget->setContentsMargins(8,8,8,8);

    ui->centralWidget->setStyleSheet("QWidget{"
                                     "background-color: #2b2b2b;}");

    // Add new verticalBox to central Widget
    vBox1 = new QVBoxLayout(ui->centralWidget);

    progressBar = new QProgressBar(ui->centralWidget);
    pButtonExit = new QPushButton(ui->centralWidget);
    pButtonStart = new QPushButton(ui->centralWidget);

    pButtonStart->setText("Starte StarCar");

    pButtonStart->setStyleSheet("QPushButton{"
                                "background-color: red;}");

    pButtonExit->setText("Beenden");
    pButtonExit->setStyleSheet("QPushButton{"
                                "background-color: red;}");

    progressBar->setRange(0,100);
    progressBar->setValue(100);
    progressBar->setTextVisible(false);

    progressBar->setStyleSheet("QProgressBar{"
                               "background-color: black;"
                               "border: 2px solid grey;"
                               "border-radius: 5px;}"

                               "QProgressBar::chunk{"
                               "background-color: #08ff08;"
                               "width: 10px;"
                               "margin: 5px;}");

    vBox1->addWidget(pButtonStart);
    vBox1->addWidget(progressBar);
    vBox1->addWidget(pButtonExit);
    vBox1->setMargin(10);

    // connect exit button to close function -> once exit is clicked the application will close
    connect(pButtonExit, SIGNAL(clicked()), this, SLOT(close()));
    connect(pButtonStart, SIGNAL(clicked()), this, SLOT(fillProgressBar()));



    return true;

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
