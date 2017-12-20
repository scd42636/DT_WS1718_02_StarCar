#include "exitwidget.h"
#include <QApplication>
#include <QProcess>

#define DEBUG

ExitWidget::ExitWidget(QWidget *parent, Alert *alertThread) : QWidget(parent)
{
    this->alertThread = alertThread;
    generateLayout();
    setupConnect();
    generateStyle();
}

void ExitWidget::generateLayout(){

    vBox1           = new QVBoxLayout(this);
    pButtonBack     = new QPushButton();
    pButtonRestart  = new QPushButton();
    pButtonShutdown = new QPushButton();

    vBox1->addWidget(pButtonBack);
    vBox1->addSpacing(5);
    vBox1->addWidget(pButtonRestart);
    vBox1->addSpacing(5);
    vBox1->addWidget(pButtonShutdown);
}

void ExitWidget::generateStyle(){

    vBox1->setAlignment(Qt::AlignHCenter);

    pButtonBack->setText("Zurück");
    pButtonBack->setObjectName("QPushButton1");

    pButtonRestart->setText("Neustart");
    pButtonShutdown->setText("Beenden");

    pButtonBack->setMinimumWidth(160);
    pButtonBack->setMinimumHeight(30);
    pButtonRestart->setMaximumWidth(160);
    pButtonRestart->setMinimumHeight(30);
    pButtonShutdown->setMaximumWidth(160);
    pButtonShutdown->setMinimumHeight(30);

    this->setStyleSheet("QPushButton{"
                            "color: green;"
                            "font-family: TimesNewRoman;"
                            "font-style: normal;"
                            "font-size: 10pt;"
                            "font-weight: bold;}");
}

void ExitWidget::setupConnect(){

    connect(pButtonBack,     SIGNAL(clicked(bool)), this, SLOT(slotGoBack()));
    connect(pButtonRestart,  SIGNAL(clicked(bool)), this, SLOT(slotRestartApplication()));
    connect(pButtonShutdown, SIGNAL(clicked(bool)), this, SLOT(slotShutdownPi()));
}

void ExitWidget::slotGoBack(){

    emit removeWindowformStack();
}

void ExitWidget::slotRestartApplication(){

    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());

}

void ExitWidget::slotShutdownPi(){

#ifdef DEBUG

    this->parentWidget()->parentWidget()->parentWidget()->close();

#endif
}
