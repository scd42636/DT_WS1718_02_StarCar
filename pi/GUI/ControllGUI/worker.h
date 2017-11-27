#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>

class Worker : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    Worker();
    ~Worker();

public slots:
    void doWork(const QString &parameter) {
        // ...
        //emit resultReady(result);
    }

signals:
    void resultReady(const QString &result);
};

#endif // WORKER_H
