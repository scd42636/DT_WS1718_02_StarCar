#ifndef EXITWIDGET_H
#define EXITWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

class ExitWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExitWidget(QWidget *parent = nullptr);

    ~ExitWidget(){

    static int const EXIT_CODE_REBOOT = -123456789;
    }

private:

    // QVBoxLayout
    QVBoxLayout *vBox1;

    // QPushButton
    QPushButton *pButtonBack;
    QPushButton *pButtonRestart;
    QPushButton *pButtonShutdown;

    // Methode
    void generateLayout();
    void setupConnect();


    void generateStyle();
signals:

    void removeWindowformStack();

public slots:

private slots:
    void goBack();
    void restartPi();
    void shutdownPi();


};

#endif // EXITWIDGET_H
