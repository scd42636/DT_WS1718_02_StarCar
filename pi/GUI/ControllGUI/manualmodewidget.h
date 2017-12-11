#ifndef MANUALMODEWIDGET_H
#define MANUALMODEWIDGET_H

#include <QObject>
#include <QWidget>
#include <alert.h>
#include <manualmode.h>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>

class ManualModeWidget : public QWidget
{
    Q_OBJECT

public:

    explicit ManualModeWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr);
    ~ManualModeWidget();

signals:

    void removeWindowformStack();

public slots:

    void pButtonGoBackPushed();
    void pButtonNextPushed();

private slots:

    void blinkLable();

private:

    // Thread
    Alert       *alertThread;
    ManualMode  *manualMode;

    // QPushButton
    QPushButton     *pButtonGoBack;
    QPushButton     *pButtonNext;

    // QVBoxLayout
    QVBoxLayout     *vBox1;
    QVBoxLayout     *vBoxleftImage;
    QVBoxLayout     *vBoxrightImage;

    // QHBoxLayout
    QHBoxLayout     *hBoxImages;

    // QLabel
    QLabel          *lblInfo;
    QLabel          *lblImageleft;
    QLabel          *lblImageright;
    QLabel          *lblUp;
    QLabel          *lblDown;
    QLabel          *lblLeft;
    QLabel          *lblright;

    // QTimer
    QTimer          *blinkTimer;

    // Vars
    double fontSize = 12.5;

    // Method
    void generateLayout();
    void generateStyle();
    void setupConnects();

};

#endif // MANUALMODEWIDGET_H
