#ifndef CLOCKCONTROLMODEWIDGET_H
#define CLOCKCONTROLMODEWIDGET_H

#define IBCNOTWORKING

#include <QObject>
#include <QWidget>
#include <alert.h>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QImage>

#ifndef IBCNOTWORKING

   #include "../IBP/IBC.hpp"

#else

   #include "../StarCarSerialProtocol/StarcarProtocol.h"

#endif

class ClockControlModeWidget : public QWidget
{
    Q_OBJECT

public:

#ifndef IBCNOTWORKING

   explicit ClockControlModeWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr, IBC *IBCPointer = nullptr);

#else

    explicit ClockControlModeWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr,
                                     StarCarProtocol *starcarprotocol = nullptr);

#endif


    ~ClockControlModeWidget();

signals:

    void removeWindowformStack();
    void showsensorvalueswidget();

private slots:

    void slotpButtonGoBackPushed();
    void slotpButtonNextPushed();
    void slotBlinklblInfo();
    void slotBlinkArrows();
    void slotShowSensorValues();

private:

    // Thread
    Alert             *alertThread;

    // QPushButton
    QPushButton     *pButtonGoBack;
    QPushButton     *pButtonNext;

    // QVBoxLayout
    QVBoxLayout     *vBox1;
    QVBoxLayout     *vBoxLeftTexts;
    QVBoxLayout     *vBoxRightTexts;
    QVBoxLayout     *vBoxLeftImagesArrow;
    QVBoxLayout     *vBoxRightImagesArrow;

    // QHBoxLayout
    QHBoxLayout     *hBoxButtonsBottom;
    QHBoxLayout     *hBoxImages;

    // QLabel
    QLabel          *lblInfo;
    QLabel          *lblImageViewClockLeft;
    QLabel          *lblImageViewClockRight;
    QLabel          *lblTextSpeedUp;
    QLabel          *lblTextBreak;
    QLabel          *lblTextTurnLeft;
    QLabel          *lblTextTurnRight;
    QLabel          *lblArrowUpRight;
    QLabel          *lblArrowDownRight;
    QLabel          *lblArrowUpLeft;
    QLabel          *lblArrowDownLeft;

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

    void setArrowPicsToLabel(QLabel *lbl, QString path);
    void setStyletoLabel(QLabel *lbl, Qt::Alignment align);
};

#endif // CLOCKCONTROLMODEWIDGET_H
