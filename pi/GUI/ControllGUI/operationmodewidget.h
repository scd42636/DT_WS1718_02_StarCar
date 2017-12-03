#ifndef OPERATIONMODEWIDGET_H
#define OPERATIONMODEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class OperationModeWidget : public QWidget
{
    Q_OBJECT

public:

    explicit OperationModeWidget(QWidget *parent = nullptr);
    ~OperationModeWidget();

signals:

    void removeWindowformStack();

public slots:

private:

    // QHBoxLayout
    QHBoxLayout     *hBox1;

    // QVBoxLayout
    QVBoxLayout     *vBox1;

    // QPushButton
    QPushButton     *pButtonManualMode;
    QPushButton     *pButtonAutomaticMode;

    // QLabel
    QLabel          *lblHeadline;

    // Methode
    void generateLayout();
    void generateStyle();
    void setupConnects();
};

#endif // OPERATIONMODEWIDGET_H
