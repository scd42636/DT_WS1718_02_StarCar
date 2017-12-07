#ifndef MANUALMODEWIDGET_H
#define MANUALMODEWIDGET_H

#include <QObject>
#include <QWidget>
#include <alert.h>
#include <manualmode.h>

class ManualModeWidget : public QWidget
{
    Q_OBJECT

public:

    explicit ManualModeWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr);

signals:

    void removeWindowformStack();

public slots:

private:

    // Thread
    Alert       *alertThread;
    ManualMode  *manualMode;

};

#endif // MANUALMODEWIDGET_H
