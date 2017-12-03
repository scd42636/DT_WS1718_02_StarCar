#ifndef ALERTWIDGET_H
#define ALERTWIDGET_H

#include <QObject>
#include <QWidget>
#include "alert.h"

class AlertWidget : public QWidget
{
    Q_OBJECT

public:

    explicit AlertWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr);
    ~AlertWidget();

signals:

    void removeWindowformStack();

public slots:

private:

    // Thread
    Alert *alertThread;

};

#endif // ALERTWIDGET_H
