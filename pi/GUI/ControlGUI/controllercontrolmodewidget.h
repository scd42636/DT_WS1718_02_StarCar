#ifndef CONTROLLERCONTROLMODEWIDGET_H
#define CONTROLLERCONTROLMODEWIDGET_H

#define IBCNOTWORKING

#include <QObject>
#include <QWidget>
#include "alert.h"
#include <QVBoxLayout>
#include <QLabel>
#include <sensorvalueswidget.h>

#ifndef IBCNOTWORKING

   #include "../IBP/IBC.hpp"

#else

   #include "../StarCarSerialProtocol/StarcarProtocol.h"

#endif

class ControllerControlModeWidget : public QWidget
{
    Q_OBJECT

public:


#ifndef IBCNOTWORKING

   explicit ControllerControlModeWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr, IBC *IBCPointer = nullptr);

#else

    explicit ControllerControlModeWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr,
                                         StarCarProtocol *starcarprotocol = nullptr);

#endif

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

#ifndef IBCNOTWORKING

    // IBC
    IBC             *IBCPointer;

#else

    //Protocol
    StarCarProtocol *starcarprotocol;

#endif

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
