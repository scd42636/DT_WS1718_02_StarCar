#ifndef SENSORVALUESWIDGET_H
#define SENSORVALUESWIDGET_H

#include <QObject>
#include <QWidget>
#include <alert.h>
#include <QVBoxLayout>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <threadlidar.h>
#include <../StarCarSerialProtocol/starcarprotocol.h>

class SensorValuesWidget : public QWidget
{
    Q_OBJECT

public:

    explicit SensorValuesWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr,
                                QString pButtonGoBackText = "Zurück zur Moduswahl",
                                StarCarProtocol *starcarprotocol = nullptr);

signals:

    void removeWindowfromStack();

public slots:

private slots:

    void slotpButtonGoBackPushed();
    void slotQuerySensorValues();

private:

    // Thread
    Alert           *alertThread;
    ThreadLidar     *threadLidar;

    // QVBoxLayout
    QVBoxLayout     *vBox1;
    QVBoxLayout     *vBoxLabelDescription;
    QVBoxLayout     *vBoxLabelValues;

    // QHBoxLayout
    QHBoxLayout     *hBox1;

    // QPushButton
    QPushButton     *pButtonGoBack;

    // QLabel
    QLabel          *lblUltraFront;
    QLabel          *lblUltraBack;
    QLabel          *lblcompass;
    QLabel          *lblacceleration;
    QLabel          *lblUWB;

    QLabel          *lblUltraFrontValue;
    QLabel          *lblUltraBackValue;
    QLabel          *lblcompassValue;
    QLabel          *lblaccelerationValue;
    QLabel          *lblUWBValue;

    // QVector
    QVector<QLabel *> lblValues;

    // QString
    QString         pButtonGoBackText;

    // QTimer
    QTimer          *lidarTimer;

    //QTimer
    QTimer          *QuerySensorValuesTimer;

    // Method
    void generateLayout();
    void setupConnects();
    void generateStyle();

    StarCarProtocol         *starcarprotocol;
};

#endif // SENSORVALUESWIDGET_H
