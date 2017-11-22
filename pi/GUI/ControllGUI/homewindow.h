#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>

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
    void on_pButtonStart_clicked();

    void on_pButtonStop_clicked();

    void on_pButtonExit_clicked();

private:
    Ui::HomeWindow *ui;
};

#endif // HOMEWINDOW_H
