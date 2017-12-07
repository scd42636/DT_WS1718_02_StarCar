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

    ~Alert();

    QVector<QString> getwarMsg();
    QVector<QString> geterrMsg();

public slots:

    void process();
    void finishAlert();
    void changeAlertIcon();
    void clearWarning();
    void clearError();

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

    QVector<QString> errMsg;
    QVector<QString> warMsg;

};

#endif // ALERT_H
