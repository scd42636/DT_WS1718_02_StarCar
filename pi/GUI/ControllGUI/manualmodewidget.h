#ifndef MANUALMODEWIDGET_H
#define MANUALMODEWIDGET_H

#include <QObject>
#include <QWidget>

class ManualModeWidget : public QWidget
{
    Q_OBJECT

public:

    explicit ManualModeWidget(QWidget *parent = nullptr);

signals:

    void removeWindowformStack();

public slots:

};

#endif // MANUALMODEWIDGET_H
