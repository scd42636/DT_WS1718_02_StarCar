#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QObject>
#include <QHBoxLayout>
#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QThread>
#include "alert.h"
#include "startwidget.h"
#include "exitwidget.h"
#include "alertwidget.h"
#include "operationmodewidget.h"
#include "clockcontrolmodewidget.h"
#include "controllercontrolmodewidget.h"
#include "sensorvalueswidget.h"
#include "../../IBP/IBC.hpp"

namespace Ui {
class HomeWindow;
}

class HomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HomeWindow(QWidget *parent = 0);
    ~HomeWindow();

    // Thread
    Alert           *alertThread;

    // Timer
    QTimer          *alertTimer;

public slots:

    void removeActiveWidget();

private slots:

    void slotShowExitWidget();
    void slotShowAlertWidget();
    void slotShowOperationModeWidget();
    void slotShowClockControlModeWidget();
    void slotShowControllerControlModeWidget();
    void slotShowSensorValuesWidget();
    void slotShowStartWidget();
    void slotShowSensorValuesWidgetAfterControlMode();

private:

    // Window
    Ui::HomeWindow  *ui;

    // Layout-Box
    QVBoxLayout     *centralVBox;
    QHBoxLayout     *hBox1;

    // StackedWidget
    QStackedWidget  *mainStackedWidget;

    // Widget
    QWidget                     *startWidget;
    ExitWidget                  *exitWidget;
    AlertWidget                 *alertWidget = nullptr;
    OperationModeWidget         *operationModeWidget;
    SensorValuesWidget          *sensorValuesWidget;
    ClockControllModeWidget     *clockcontrolModeWidget;
    ControllerControlModeWidget *controllercontrolModeWidget;

    // Button
    QPushButton     *pButtonExit;
    QPushButton     *pButtonAlert;

    // Label
    QLabel          *lblHeadline;

    // IBC
    IBC             *IBCPointer;

    // Vars
    bool            IBCactive = false;

private:

    // Method
    void generateLayout();
    void generateStyle();
    void setupConnects();
    void createAlertThread();
    void addWidgetToMainStackWidget(QWidget *widget);
};

#endif // HOMEWINDOW_H
