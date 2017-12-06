#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QPushButton>
#include <QObject>
#include <QHBoxLayout>
#include <QMainWindow>
#include <alert.h>

class StartWidget : public QWidget
{
    Q_OBJECT

public:

    explicit StartWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr);

    ~StartWidget();

private:

    // ProgressBar
    QProgressBar    *progressBar;

    // Button
    QPushButton     *pButtonStart;

    // Vertical-Box
    QVBoxLayout     *vBox1;

    //Timer
    QTimer          *progressBarTimer;

    // Thread
    Alert           *alertThread;

    // Methodes
    void generateLayout();
    void setupProgressBar();
    void setupConnects();
    void generateStyle();

signals:

    void showOperationMode();
    void removeWindowformStack();

public slots:

private slots:

    void startProgressBar();
    void fillProgressBar();
};

#endif // STARTWIDGET_H
