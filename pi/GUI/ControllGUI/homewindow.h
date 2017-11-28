#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QObject>
#include <QHBoxLayout>
#include <QWidget>
#include <QStackedWidget>

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
    void closeApp();

protected:
    void addWidgetToMainStackWidget(QWidget *widget);

private slots:

private:

    // Window
    Ui::HomeWindow  *ui;

    // Vertical-Box
    QVBoxLayout     *centralVBox;

    // StackedWidget
    QStackedWidget  *mainStackedWidget;

    // Widget
    QWidget         *startWidget;

    // Methods
    void generateStartLayout();
    void styleWindow();
};

#endif // HOMEWINDOW_H
