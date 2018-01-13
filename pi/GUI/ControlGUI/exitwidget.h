#ifndef EXITWIDGET_H
#define EXITWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <alert.h>
#include <../StarCarSerialProtocol/starcarprotocol.h>

class ExitWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExitWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr,
                        StarCarProtocol **starcarprotocol = nullptr, bool protocolActive = false);

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

    //Protocol
    StarCarProtocol **starcarprotocol;

    //Vars
    bool            protocolActive = false;

signals:

    void removeWindowformStack();

private slots:
    void slotGoBack();
    void slotRestartApplication();
    void slotShutdownPi();


};

#endif // EXITWIDGET_H
