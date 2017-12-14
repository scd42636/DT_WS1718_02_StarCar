#ifndef CONTROLLERCONTROLMODEWIDGET_H
#define CONTROLLERCONTROLMODEWIDGET_H

#include <QObject>
#include <QWidget>
#include "alert.h"
#include <QVBoxLayout>
#include <QLabel>
#include <sensorvalueswidget.h>
#include <../../IBP/IBC.hpp>
#include <../../IBP/IBC_Packet.hpp>
#include <../../IBP/Serial.hpp>

class ControllerControlModeWidget : public QWidget
{
    Q_OBJECT

public:

    explicit ControllerControlModeWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr, IBC *IBCPointer = nullptr);

signals:

    void removeWindowfromStack();
    void showsensorvalueswidget();

public slots:

    void pButtonGoBackPushed();
    void blinklblInfo();
    void pButtonNextPushed();

private slots:

    void blinkArrows();
    void slotShowSensorValues();

private:

    // Thread
    Alert       *alertThread;

    // QPushButton
    QPushButton     *pButtonGoBack;
    QPushButton     *pButtonNext;

    // QVBoxLayout
    QVBoxLayout     *vBox1;
    QVBoxLayout     *vBoxRightTexts;
    QVBoxLayout     *vBoxLeftTexts;
    QVBoxLayout     *vBoxRightImagesArrow;
    QVBoxLayout     *vBoxLeftImagesArrow;
    QVBoxLayout     *vBoxLeftImagesAndTexts;

    // QHBoxLayout
    QHBoxLayout     *hBoxImages;
    QHBoxLayout     *hBoxArrowsLeft;
    QHBoxLayout     *hBoxTextsLeft;
    QHBoxLayout     *hboxButtonsBottom;

    // QLabel
    QLabel          *lblInfo;
    QLabel          *lblImageViewController;
    QLabel          *lblTextSpeedUp;
    QLabel          *lblTextBreak;
    QLabel          *lblTextTurnLeft;
    QLabel          *lblTextTurnRight;
    QLabel          *lblArrowUp;
    QLabel          *lblArrowDown;
    QLabel          *lblArrowLeft;
    QLabel          *lblArrowRight;

    // QTimer
    QTimer          *blinkTimer;

    // IBC
    IBC             *IBCPointer;
    Packet          *IBCPaket;
    Serial          *PortToArduino;

    // Vars
    double fontSize = 10;
    bool   pButtonNextRemoved = false;

    // Method
    void generateLayout();
    void generateStyle();
    void setupConnects();
    void createControllAnimation();
    void setStyletoLabel(QLabel *lbl, Qt::Alignment align);
    void setArrowPicsToLabel(QLabel *lbl, QString path, int height, int width);
};

#endif // CONTROLLERCONTROLMODEWIDGET_H
