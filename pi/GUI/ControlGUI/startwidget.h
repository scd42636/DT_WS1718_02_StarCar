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

#ifndef IBCNOTWORKING

   #include "../IBP/IBC.hpp"

#else

   #include "../StarCarSerialProtocol/StarcarProtocol.h"

#endif

class StartWidget : public QWidget
{
    Q_OBJECT

public:


#ifndef IBCNOTWORKING

   explicit StartWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr, IBC **IBCPointer = nullptr);

#else

   explicit StartWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr, StarCarProtocol **starcarprotocol = nullptr);

#endif

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

#ifndef IBCNOTWORKING

    // IBC
    IBC             **IBCPointer;

#else

    //Protocol
    StarCarProtocol **starcarprotocol;

#endif

    // Methods
    void generateLayout();
    void setupProgressBar();
    void setupConnects();
    void generateStyle();

signals:

    void showOperationMode();
    void removeWindowformStack();

private slots:

    void fillProgressBar();
    void initializeStarCar();
};

#endif // STARTWIDGET_H
