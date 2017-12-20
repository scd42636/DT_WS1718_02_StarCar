#ifndef AUTOMATICMODE_H
#define AUTOMATICMODE_H

#include <QObject>
#include <alert.h>

class AutomaticMode : public QObject
{
    Q_OBJECT

public:

    explicit AutomaticMode(Alert *alertThread = nullptr);

signals:

    void finished();

public slots:

    void startProcess();
    void finishAutomaticMode();

private:

    Alert           *alertThread;
};

#endif // AUTOMATICMODE_H
