#ifndef CONTROLLERCONTROLMODEWIDGET_H
#define CONTROLLERCONTROLMODEWIDGET_H

#include <QObject>
#include <QWidget>
#include "alert.h"
#include <QVBoxLayout>
#include <QLabel>

class ControllerControlModeWidget : public QWidget
{
    Q_OBJECT

public:

    explicit ControllerControlModeWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr);

signals:

    void removeWindowfromStack();

public slots:

    void pButtonGoBackPushed();
    void blinklblInfo();
    void pButtonNextPushed();

private:

    // Thread
    Alert       *alertThread;

    // QPushButton
    QPushButton     *pButtonGoBack;
    QPushButton     *pButtonNext;

    // QVBoxLayout
    QVBoxLayout     *vBox1;

    // QLabel
    QLabel          *lblInfo;

    // QTimer
    QTimer          *blinkTimer;

    // Vars
    double fontSize = 10;
    bool   pButtonNextRemoved = false;

    // Method
    void generateLayout();
    void generateStyle();
    void setupConnects();
};

#endif // CONTROLLERCONTROLMODEWIDGET_H
