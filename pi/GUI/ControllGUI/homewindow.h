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
#include "exitwidget.h"

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
protected:
    void addWidgetToMainStackWidget(QWidget *widget);

private slots:

    void showExitWidget();
private:

    // Window
    Ui::HomeWindow  *ui;

    // Layout-Box
    QVBoxLayout     *centralVBox;
    QHBoxLayout     *hBox1;

    // StackedWidget
    QStackedWidget  *mainStackedWidget;

    // Widget
    QWidget         *startWidget;
    ExitWidget      *exitwidget;

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
    void generateStartLayout();
    void styleWindow();
    void setupConnects();
    void createAlertThread();
};

#endif // HOMEWINDOW_H
