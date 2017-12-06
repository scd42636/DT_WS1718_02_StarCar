#ifndef ALERT_H
#define ALERT_H

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

    Alert(QPushButton *pButtonAlert);

    void fireWarning();
    void fireWarning(QString warMsg);
    void fireWarning(QString warMsg, int warNum);

    void fireError();
    void fireError(QString errMsg);
    void fireError(QString errMsg, int errNum);

    void clearWarning();
    void clearError();

    ~Alert();

public slots:

    void process();
    void finishAlert();
    void changeAlertIcon();

signals:

    void finished();
    void error(QString err);

private:

    // QPushButton
    QPushButton *pButtonAlert;

    // Vars
    int         color = WHITE;

    bool        errorIsSet = false;
    bool        warningIsSet = false;
    bool        lastTimeWhite = true;
    bool        warningAndErrorIsSet = false;
    bool        lastTimeOrange = false;

    QString     errMsg;
    QString     warMsg;

    int         errNum[25];
    int         warNum[25];

    int         errNumCount = 0;
    int         warNumCount = 0;
};

#endif // ALERT_H
