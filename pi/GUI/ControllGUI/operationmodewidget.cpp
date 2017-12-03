#include "operationmodewidget.h"

OperationModeWidget::OperationModeWidget(QWidget *parent) : QWidget(parent)
{
    generateLayout();
    generateStyle();
    setupConnects();
}

void OperationModeWidget::generateLayout(){

    vBox1 = new QVBoxLayout(this);
    lblHeadline = new QLabel();
    hBox1 = new QHBoxLayout();
    pButtonManualMode = new QPushButton();
    pButtonAutomaticMode = new QPushButton();

    vBox1->addWidget(lblHeadline);
    vBox1->addLayout(hBox1);

    hBox1->addWidget(pButtonManualMode);
    hBox1->addSpacing(30);
    hBox1->addWidget(pButtonAutomaticMode);
}

void OperationModeWidget::generateStyle(){

    vBox1->setAlignment(Qt::AlignVCenter);
    lblHeadline->setAlignment(Qt::AlignHCenter);
    hBox1->setAlignment(Qt::AlignVCenter);

    lblHeadline->setText("Betriebsmodi Auswahl");


    pButtonManualMode->setText("Manuell");
    pButtonAutomaticMode->setText("Automatik");

    pButtonManualMode->setMinimumSize(20,20);
    pButtonAutomaticMode->setMinimumSize(20,20);

    this->setStyleSheet("QPushButton{"
                        "color: green;"
                        "font-family: TimesNewRoman;"
                        "font-style: normal;"
                        "font-size: 20pt;"
                        "font-weight: bold;}");

}

void OperationModeWidget::setupConnects(){

    connect(pButtonManualMode, SIGNAL(clicked(bool)), this, SLOT(slotShowManualModeWidget()));
    connect(pButtonAutomaticMode, SIGNAL(clicked(bool)), this, SLOT(slotShowAutomaticModeWidget()));
}

void OperationModeWidget::slotShowManualModeWidget(){

    emit showmanualmodewidget();
}

void OperationModeWidget::slotShowAutomaticModeWidget(){

    emit showautomaticmodewidget();
}

OperationModeWidget::~OperationModeWidget(){

}
