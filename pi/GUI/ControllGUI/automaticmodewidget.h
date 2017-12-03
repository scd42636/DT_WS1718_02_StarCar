#ifndef AUTOMATICMODEWIDGET_H
#define AUTOMATICMODEWIDGET_H

#include <QObject>
#include <QWidget>

class AutomaticModeWidget : public QWidget
{
    Q_OBJECT

public:

    explicit AutomaticModeWidget(QWidget *parent = nullptr);

signals:

    void removeWindowformStack();

public slots:

};

#endif // AUTOMATICMODEWIDGET_H
