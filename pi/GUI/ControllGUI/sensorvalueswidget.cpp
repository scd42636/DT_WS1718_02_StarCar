#include "sensorvalueswidget.h"

SensorValuesWidget::SensorValuesWidget(QWidget *parent, Alert *alertThread, QString pButtonGoBackText) : QWidget(parent)
{
    this->alertThread = alertThread;
    this->pButtonGoBackText = pButtonGoBackText;

    generateLayout();
    setupConnects();
    generateStyle();
}

void SensorValuesWidget::generateLayout(){

    vBox1 = new QVBoxLayout(this);
    hBox1 = new QHBoxLayout();

    vBoxLabelDescription    = new QVBoxLayout();
    vBoxLabelValues         = new QVBoxLayout();

    lblUltraFront           = new QLabel();
    lblUltraBack            = new QLabel();
    lblcompass              = new QLabel();
    lblacceleration         = new QLabel();
    lblUWB                  = new QLabel();

    lblUltraFrontValue      = new QLabel();
    lblUltraBackValue       = new QLabel();
    lblcompassValue         = new QLabel();
    lblaccelerationValue    = new QLabel();
    lblUWBValue             = new QLabel();

    pButtonGoBack           = new QPushButton();

    vBox1->addLayout(hBox1);

    hBox1->addLayout(vBoxLabelDescription);
    hBox1->addLayout(vBoxLabelValues);

    vBoxLabelDescription->addWidget(lblUltraFront);
    vBoxLabelDescription->addWidget(lblUltraBack);
    vBoxLabelDescription->addWidget(lblcompass);
    vBoxLabelDescription->addWidget(lblacceleration);
    vBoxLabelDescription->addWidget(lblUWB);

    vBoxLabelValues->addWidget(lblUltraFrontValue);
    vBoxLabelValues->addWidget(lblUltraBackValue);
    vBoxLabelValues->addWidget(lblcompassValue);
    vBoxLabelValues->addWidget(lblaccelerationValue);
    vBoxLabelValues->addWidget(lblUWBValue);

    vBox1->addWidget(pButtonGoBack);

    lblValues.append(lblUltraFrontValue);
    lblValues.append(lblUltraBackValue);
    lblValues.append(lblcompassValue);
    lblValues.append(lblaccelerationValue);
    lblValues.append(lblUWBValue);

    for (QLabel *lables : lblValues){
        lables->setText("--");
    }
}

void SensorValuesWidget::setupConnects(){

    connect(pButtonGoBack, SIGNAL(clicked(bool)), this, SLOT(pButtonGoBackPushed()));
}

void SensorValuesWidget::generateStyle(){

    this->setStyleSheet("QLabel{"
                            "color: white;"
                            "font-family: TimesNewRoman;"
                            "font-style: normal;"
                            "font-size: 10pt;"
                            "font-weight: bold;}"
                        "QPushButton{"
                            "color: green;"
                            "font-family: TimesNewRoman;"
                            "font-style: normal;"
                            "font-size: 12pt;"
                            "font-weight: bold;}");

    vBox1->setContentsMargins(8,0,8,0);

    lblUltraFront->setText("Ultraschall Vorne: ");
    lblUltraBack->setText("Ultraschall Hinten: ");
    lblcompass->setText("Kompass: ");
    lblacceleration->setText("Beschleunigung: ");
    lblUWB->setText("UWB: ");

    pButtonGoBack->setText(pButtonGoBackText);
}


void SensorValuesWidget::pButtonGoBackPushed(){

    emit removeWindowfromStack();
}
