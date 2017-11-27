#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QPushButton>
#include <QObject>
#include <QLabel>
#include <QHBoxLayout>

namespace Ui {
class HomeWindow;
}

class HomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HomeWindow(QWidget *parent = 0);
    ~HomeWindow();


private slots:

    void fillProgressBar();
    void closeStarCar();

private:

    //Window
    Ui::HomeWindow  *ui;

    //Label
    QLabel          *lblHeadline;

    //ProgressBar
    QProgressBar    *progressBar;

    //Buttons
    QPushButton     *pButtonExit;
    QPushButton     *pButtonStart;
    QPushButton     *pButtonAlert;

    //Vertical-Box
    QVBoxLayout     *vBox1;

    //Horizontal-Box
    QHBoxLayout     *hBox1;

    //Methods
    void generateStartLayout();
    void styleStartLayout();
};

#endif // HOMEWINDOW_H
