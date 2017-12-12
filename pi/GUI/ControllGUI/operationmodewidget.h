#ifndef OPERATIONMODEWIDGET_H
#define OPERATIONMODEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <alert.h>

class OperationModeWidget : public QWidget
{
    Q_OBJECT

public:

    explicit OperationModeWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr);
    ~OperationModeWidget();

signals:

    void removeWindowformStack();
    void showclockcontrollmodewidget();
    void showautomaticmodewidget();
    void showcontrollercontrolmodewidget();

public slots:


private slots:

    void slotShowClockControlModeWidget();
    void slotShowAutomaticModeWidget();
    void slotShowControllerControlModeWidget();


private:

    // QHBoxLayout
    QHBoxLayout     *hBox1;

    // QVBoxLayout
    QVBoxLayout     *vBox1;

    // QPushButton
    QPushButton     *pButtonClockControlMode;
    QPushButton     *pButtonAutomaticMode;
    QPushButton     *pButtonControllerControlMode;

    // QLabel
    QLabel          *lblHeadline;

    // Methode
    void generateLayout();
    void generateStyle();
    void setupConnects();

    //Thread
    Alert           *alertThread;
};

#endif // OPERATIONMODEWIDGET_H
