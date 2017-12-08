#ifndef MANUALMODE_H
#define MANUALMODE_H

#include <QObject>
#include <alert.h>

class ManualMode : public QObject
{
    Q_OBJECT

public:

    explicit ManualMode(Alert *alertThread);

signals:

    void finished();

public slots:

    void finishAutomaticMode();
    void startProcess();

private:

    // Thread
    Alert       *alertThread;

};

#endif // MANUALMODE_H
