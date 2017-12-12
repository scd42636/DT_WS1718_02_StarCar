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

    void blinklblInfo();

    void blinkArrows();
private:

    // Thread
    Alert       *alertThread;
    ManualMode  *manualMode;

    // QPushButton
    QPushButton     *pButtonGoBack;
    QPushButton     *pButtonNext;

    // QVBoxLayout
    QVBoxLayout     *vBox1;
    QVBoxLayout     *vBoxLeftTexts;
    QVBoxLayout     *vBoxRightTexts;
    QVBoxLayout     *vBoxLeftImagesArrow;
    QVBoxLayout     *vBoxRightImagesArrow;

    // QHBoxLayout
    QHBoxLayout     *hBoxImages;

    // QLabel
    QLabel          *lblInfo;
    QLabel          *lblImageViewClockLeft;
    QLabel          *lblImageViewClockRight;
    QLabel          *lblTextSpeedUp;
    QLabel          *lblTextBreak;
    QLabel          *lblTextTurnLeft;
    QLabel          *lblTextTurnRight;
    QLabel          *lblArrowUpRight;
    QLabel          *lblArrowDownRight;
    QLabel          *lblArrowUpLeft;
    QLabel          *lblArrowDownLeft;

    // QTimer
    QTimer          *blinkTimer;

    // Vars
    double fontSize = 10;
    bool   pButtonNextRemoved = false;

    // Method
    void generateLayout();
    void generateStyle();
    void setupConnects();
    void createControllAnimation();

    void setArrowPicsToLabel(QLabel *lbl, QString path);
    void setStyletoLabel(QLabel *lbl, Qt::Alignment align);
};

#endif // MANUALMODEWIDGET_H
