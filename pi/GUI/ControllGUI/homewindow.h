#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QPushButton>
#include <QObject>

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

private:

    Ui::HomeWindow  *ui;
    QProgressBar    *progressBar;
    QVBoxLayout     *vBox1;
    QPushButton     *pButtonExit;
    QPushButton     *pButtonStart;

    bool generateStartLayout();

};

#endif // HOMEWINDOW_H
