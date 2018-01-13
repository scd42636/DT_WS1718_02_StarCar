#ifndef CONTROLLERCONTROLMODEWIDGET_H
#define CONTROLLERCONTROLMODEWIDGET_H

#include <QObject>
#include <QWidget>
#include "alert.h"
#include <QVBoxLayout>
#include <QLabel>
#include <sensorvalueswidget.h>
#include <../StarCarSerialProtocol/starcarprotocol.h>

class ControllerControlModeWidget : public QWidget
{
    Q_OBJECT

public:

    explicit ControllerControlModeWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr,
                                         StarCarProtocol *starcarprotocol = nullptr);
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

    // Protocol
    StarCarProtocol *starcarprotocol;

    // Vars
    double fontSize = 10;
    bool   pButtonNextRemoved = false;
    bool   lastTimeWasShort = false;

    // Method
    void setupWidget();
    void createControllAnimation();
    void setStyletoLabel(QLabel *lbl, Qt::Alignment align);
    void setArrowPicsToLabel(QLabel *lbl, QString path, int height, int width);
};

#endif // CONTROLLERCONTROLMODEWIDGET_H
