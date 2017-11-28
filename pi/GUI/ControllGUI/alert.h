#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QPushButton>
#include <QTimer>

class Alert : public QObject {

    Q_OBJECT

public:

    Alert(QPushButton *alert);
    ~Alert();

public slots:

    void process();
    void finishWorker();
    void changeAlertIcon();

signals:

    void finished();
    void error(QString err);

private:

    QPushButton *alert;
    int         red = 0;
};

#endif // WORKER_H
