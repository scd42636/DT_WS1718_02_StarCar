#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QPushButton>
#include <QObject>
#include <QLabel>
#include <QHBoxLayout>
#include <QWidget>
#include <QTimer>
#include <QThread>
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


protected:
    void addWidgetToMainStackWidget(QWidget *widget);

private slots:

    void fillProgressBar();
    void closeStarCar();

    void showAlert();
private:

    // Window
    Ui::HomeWindow  *ui;

    // Label
    QLabel          *lblHeadline;

    // ProgressBar
    QProgressBar    *progressBar;

    // Buttons
    QPushButton     *pButtonExit;
    QPushButton     *pButtonStart;
    QPushButton     *pButtonAlert;

    // Vertical-Box
    QVBoxLayout     *centralVBox;
    QVBoxLayout     *vBox1;

    // Horizontal-Box
    QHBoxLayout     *hBox1;

    // StackedWidget
    QStackedWidget  *mainStackedWidget;

    // Widget
    QWidget         *startWidget;

    // Methods
    void generateStartLayout();
    void styleStartLayout();
    void generateGUIElementsforStartWidget();
};

#endif // HOMEWINDOW_H
