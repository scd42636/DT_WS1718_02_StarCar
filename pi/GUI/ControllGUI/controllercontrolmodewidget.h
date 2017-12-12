#ifndef CONTROLLERCONTROLMODEWIDGET_H
#define CONTROLLERCONTROLMODEWIDGET_H

#include <QObject>
#include <QWidget>
#include "alert.h"
#include <QVBoxLayout>
#include <QLabel>

class ControllerControlModeWidget : public QWidget
{
    Q_OBJECT

public:

    explicit ControllerControlModeWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr);

signals:

    void removeWindowfromStack();

public slots:

    void pButtonGoBackPushed();
    void blinklblInfo();
    void pButtonNextPushed();

private slots:

    void blinkArrows();

private:

    // Thread
    Alert       *alertThread;

    // QPushButton
    QPushButton     *pButtonGoBack;
    QPushButton     *pButtonNext;

    // QVBoxLayout
    QVBoxLayout     *vBox1;
    QVBoxLayout     *vBoxRightTexts;
    QVBoxLayout     *vBoxLeftTexts;
    QVBoxLayout     *vBoxRightImagesArrow;
    QVBoxLayout     *vBoxLeftImagesArrow;
    QVBoxLayout     *vBoxLeftImagesAndTexts;

    // QHBoxLayout
    QHBoxLayout     *hBoxImages;
    QHBoxLayout     *hBoxArrowsLeft;
    QHBoxLayout     *hBoxTextsLeft;

    // QLabel
    QLabel          *lblInfo;
    QLabel          *lblImageViewController;
    QLabel          *lblTextSpeedUp;
    QLabel          *lblTextBreak;
    QLabel          *lblTextTurnLeft;
    QLabel          *lblTextTurnRight;
    QLabel          *lblArrowUp;
    QLabel          *lblArrowDown;
    QLabel          *lblArrowLeft;
    QLabel          *lblArrowRight;

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
    void setStyletoLabel(QLabel *lbl, Qt::Alignment align);
    void setArrowPicsToLabel(QLabel *lbl, QString path, int height, int width);
};

#endif // CONTROLLERCONTROLMODEWIDGET_H
