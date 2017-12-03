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
    void restartApplication();
    void shutdownPi();


};

#endif // EXITWIDGET_H
