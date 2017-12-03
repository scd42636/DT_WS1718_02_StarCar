#ifndef MANUALMODE_H
#define MANUALMODE_H

#include <QObject>
#include <QWidget>

class ManualMode : public QWidget
{
    Q_OBJECT
public:
    explicit ManualMode(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // MANUALMODE_H