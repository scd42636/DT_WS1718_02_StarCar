#ifndef CONTROLLERCONTROLMODEWIDGET_H
#define CONTROLLERCONTROLMODEWIDGET_H

#include <QObject>
#include <QWidget>
#include "alert.h"
#include <QVBoxLayout>
#include <QLabel>
#include <sensorvalueswidget.h>
#include <threadlidar.h>
#include <../../IBP/IBC.hpp>
#include <../../IBP/IBC_Packet.hpp>
#include <../../IBP/Serial.hpp>

class ControllerControlModeWidget : public QWidget
{
    Q_OBJECT

public:

    explicit ControllerControlModeWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr, IBC *IBCPointer = nullptr, Serial *SerialPortArduino = nullptr);
    ~ControllerControlModeWidget();

signals:

    void removeWindowfromStack();
    void showsensorvalueswidget();

private slots:

    void slotpButtonGoBackPushed();
    void slotpButtonNextPushed();
    void slotBlinkArrows();
    void slotShowSensorValues();
    void slotBlinklblInfo();

private:

    // Thread
    Alert       *alertThread;
    ThreadLidar *threadLidar;

    // QPushButton
    QPushButton     *pButtonGoBack;
    QPushButton     *pButtonNext;

    // QVBoxLayout
    QVBoxLayout     *vBox1;
    QVBoxLayout     *vBoxLeftControl;
    QVBoxLayout     *vBoxRightControl;

    // QHBoxLayout

    QHBoxLayout     *hBoxImages;
    QHBoxLayout     *hBoxLeftArrows;
    QHBoxLayout     *hBoxLeftTexts;
    QHBoxLayout     *hBoxRightArrowsAndTexts;
    QHBoxLayout     *hBoxController;

    QHBoxLayout     *hboxButtonsBottom;

    // QLabel
    QLabel          *lblInfo;
    QLabel          *lblImageViewController;
    QLabel          *lblTextSpeedUp;
    QLabel          *lblTextBreak;
    QLabel          *lblTextTurnLeft;
    QLabel          *lblTextTurnRight;
    QLabel          *lblArrowDown2;
    QLabel          *lblArrowDown;
    QLabel          *lblArrowLeft;
    QLabel          *lblArrowRight;

    // QTimer
    QTimer          *blinkTimer;
    QTimer          *lidarTimer;

    // IBC
    IBC             *IBCPointer;
    Serial          *SerialPortArduino;

    // Vars
    double fontSize = 10;
    bool   pButtonNextRemoved = false;

    // Method
    void setupWidget();
    void createControllAnimation();
    void setStyletoLabel(QLabel *lbl, Qt::Alignment align);
    void setArrowPicsToLabel(QLabel *lbl, QString path, int height, int width);
};

#endif // CONTROLLERCONTROLMODEWIDGET_H
