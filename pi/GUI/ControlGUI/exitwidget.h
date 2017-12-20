#ifndef EXITWIDGET_H
#define EXITWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <alert.h>
#include <../../IBP/IBC.hpp>

class ExitWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExitWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr, IBC *IBCPointer = nullptr);

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

    //IBC

    IBC             *IBCPointer;

signals:

    void removeWindowformStack();

private slots:
    void slotGoBack();
    void slotRestartApplication();
    void slotShutdownPi();


};

#endif // EXITWIDGET_H
