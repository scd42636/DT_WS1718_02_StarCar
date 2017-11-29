#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QPushButton>
#include <QTimer>
#include <QString>

#define WHITE   0
#define ORANGE  1
#define RED     2

class Alert : public QObject {

    Q_OBJECT

public:

    Alert(QPushButton *alert);
    ~Alert();

public slots:

    void process();
    void finishWorker();
    void changeAlertIcon();

    void fireWarning();
    void fireError();
    void clearWarning();
    void clearError();

signals:

    void finished();
    void error(QString err);

private:

    QPushButton *alert;
    int         color = WHITE;
    bool        errorIsSet = false;
    bool        warningIsSet = false;
    bool        lastTimeWhite = true;
    bool        warningAndErrorIsSet = false;
    bool        lastTimeOrange = false;
};

#endif // WORKER_H
