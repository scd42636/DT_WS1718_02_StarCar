#ifndef STARTWIDGET_H
#define STARTWIDGET_H

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
#include "../StarCarSerialProtocol/starcarprotocol.h"

class StartWidget : public QWidget
{
    Q_OBJECT

public:

    explicit StartWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr, StarCarProtocol **starcarprotocol = nullptr);

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

    // Protocol

    StarCarProtocol **starcarprotocol;

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
