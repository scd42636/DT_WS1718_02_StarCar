#ifndef EXITWIDGET_H
#define EXITWIDGET_H

#define IBCNOTWORKING

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <alert.h>
#include <QApplication>
#include <QProcess>

#ifndef IBCNOTWORKING

   #include "../IBP/IBC.hpp"

#else

   #include "../StarCarSerialProtocol/StarcarProtocol.h"

#endif

class ExitWidget : public QWidget
{
    Q_OBJECT

public:


#ifndef IBCNOTWORKING

   explicit ExitWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr, IBC **IBCPointer = nullptr, bool IBCactive = false);

#else

    explicit ExitWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr,
                        StarCarProtocol **starcarprotocol = nullptr, bool protocolActive = false);
#endif


    ~ExitWidget(){
 }

private:

    // QVBoxLayout
    QVBoxLayout     *vBox1;

    // QPushButton
    QPushButton     *pButtonBack;
    QPushButton     *pButtonRestart;
    QPushButton     *pButtonShutdown;

    // Method
    void generateLayout();
    void setupConnect();
    void generateStyle();

    //Thread
    Alert *alertThread;

#ifndef IBCNOTWORKING

    // IBC
    IBC             **IBCPointer;

    //Vars
    bool            IBCactive = false;

#else

    //Protocol
    StarCarProtocol **starcarprotocol;

    //Vars
    bool            protocolActive = false;

#endif


signals:

    void removeWindowformStack();

private slots:
    void slotGoBack();
    void slotRestartApplication();
    void slotShutdownPi();


};

#endif // EXITWIDGET_H
