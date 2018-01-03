#ifndef CLOCKCONTROLMODEWIDGET_H
#define CLOCKCONTROLMODEWIDGET_H

#include <QObject>
#include <QWidget>
#include <alert.h>
#include <clockcontrolmode.h>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <../../IBP/IBC.hpp>
#include <../../IBP/IBC_Packet.hpp>
#include <../../IBP/Serial.hpp>

class ClockControllModeWidget : public QWidget
{
    Q_OBJECT

public:

    explicit ClockControllModeWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr,
                                     IBC *IBCPointer = nullptr);
    ~ClockControllModeWidget();

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
    ClockControlMode  *manualMode;

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

    // IBC
    IBC             *IBCPointer;
    Serial          *SerialPortArduino;

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
