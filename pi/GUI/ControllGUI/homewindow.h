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

namespace Ui {
class HomeWindow;
}

class HomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HomeWindow(QWidget *parent = 0);
    ~HomeWindow();

public slots:

    void removeActiveWidget();
    void removeExitWidget();
    void removeAlertWidget();
    void removeOperationModeWidget();

protected:

    void addWidgetToMainStackWidget(QWidget *widget);

private slots:

    void showExitWidget();
    void showAlertWidget();
    void showOperationModeWidget();

private:

    // Window
    Ui::HomeWindow  *ui;

    // Layout-Box
    QVBoxLayout     *centralVBox;
    QHBoxLayout     *hBox1;

    // StackedWidget
    QStackedWidget  *mainStackedWidget;

    // Widget
    QWidget             *startWidget;
    ExitWidget          *exitWidget;
    AlertWidget         *alertWidget;
    OperationModeWidget *operationModeWidget;

    // Button
    QPushButton     *pButtonExit;
    QPushButton     *pButtonAlert;

    // Label
    QLabel          *lblHeadline;

    // Timer
    QTimer          *alertTimer;

public:

    // Thread
    Alert           *alertThread;

private:

    // Methods
    void generateLayout();
    void generateStyle();
    void setupConnects();
    void createAlertThread();
};

#endif // HOMEWINDOW_H
