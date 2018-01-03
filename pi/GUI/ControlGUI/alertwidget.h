#ifndef ALERTWIDGET_H
#define ALERTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QListView>
#include <QStringList>
#include <QStringListModel>
#include <QVBoxLayout>
#include "alert.h"

class AlertWidget : public QWidget
{
    Q_OBJECT

public:

    explicit AlertWidget(QWidget *parent = nullptr, Alert *alertThread = nullptr);
    ~AlertWidget();

signals:

    void removeWindowformStack();

private:

    // QPushButton

    QPushButton         *checkWarErr;

    // Thread
    Alert               *alertThread;

    // QStringList
    QStringListModel    *stringListModel;
    QStringList         *stringList;
    QListView           *listView;

    // QVBoxLayout
    QVBoxLayout         *vBox1;

    // Method
    void generateLayout();
    void generateStyle();
    void setupConnects();
};

#endif // ALERTWIDGET_H
