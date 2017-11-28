#ifndef ALERTWIDGET_H
#define ALERTWIDGET_H

#include <QObject>
#include <QWidget>

class AlertWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AlertWidget(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // ALERTWIDGET_H
