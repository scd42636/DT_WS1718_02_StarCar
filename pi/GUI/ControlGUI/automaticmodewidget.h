#ifndef AUTOMATICMODEWIDGET_H
#define AUTOMATICMODEWIDGET_H

#include <QObject>
#include <QWidget>
#include <alert.h>
#include <automaticmode.h>

class AutomaticModeWidget : public QWidget
{
    Q_OBJECT

public:

    explicit AutomaticModeWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr);

signals:

    void removeWindowformStack();

public slots:

private:

    // Thread
    Alert           *alertThread;
    AutomaticMode   *automaticMode;

};

#endif // AUTOMATICMODEWIDGET_H
