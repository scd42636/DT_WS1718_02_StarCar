#ifndef ALERTWINDOW_H
#define ALERTWINDOW_H

#include <QMainWindow>

namespace Ui {
class AlertWindow;
}

class AlertWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AlertWindow(QWidget *parent = 0);
    ~AlertWindow();

private slots:

private:
    Ui::AlertWindow *ui;
};

#endif // ALERTWINDOW_H
