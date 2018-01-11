#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#define IBCNOTWORKING

#include <QObject>
#include <QWidget>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QPushButton>
#include <QObject>
#include <QThread>
#include <QHBoxLayout>
#include <QMainWindow>
#include <alert.h>
#include <initstarcar.h>
#include "../../IBP/IBC.hpp"
#include <../SerialProtocol/StreamSerialProtocol.h>
#include <../SerialProtocol/SerialPort.hpp>
#include <starcar.h>
#include <starcar.h>

class StartWidget : public QWidget
{
    Q_OBJECT

public:

    explicit StartWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr, IBC **IBCPointer = nullptr);
    explicit StartWidget(message *msg, QWidget *parent = nullptr, Alert *alertThread = nullptr, SerialPort **serialPort = nullptr
                         , StreamSerialProtocol **protocol = nullptr);
    ~StartWidget();

private:

    // ProgressBar
    QProgressBar    *progressBar;

    // Button
    QPushButton     *pButtonStart;

    // Vertical-Box
    QVBoxLayout     *vBox1;

    //Timer
    QTimer          *progressBarTimer;

    // Thread
    Alert           *alertThread;
    InitStarCar     *initStarCar;


    IBC             **IBCPointer;

    SerialPort **serialPort;
    StreamSerialProtocol **protocol;
    message *msg;


    // Methods
    void generateLayout();
    void setupProgressBar();
    void setupConnects();
    void generateStyle();

signals:

    void showOperationMode();
    void removeWindowformStack();

public slots:

private slots:

    void fillProgressBar();
    void initializeStarCar();
};

#endif // STARTWIDGET_H
