#ifndef SENSORVALUESWIDGET_H
#define SENSORVALUESWIDGET_H

#include <QObject>
#include <QWidget>
#include <alert.h>
#include <QVBoxLayout>
#include <QLabel>

class SensorValuesWidget : public QWidget
{
    Q_OBJECT

public:

    explicit SensorValuesWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr);

signals:

    void removeWindowfromStack();

public slots:

private slots:

    void pButtonGoBackPushed();

private:

    // Thread
    Alert           *alertThread;

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

    // Methode
    void generateLayout();
    void setupConnects();
    void generateStyle();

};

#endif // SENSORVALUESWIDGET_H
