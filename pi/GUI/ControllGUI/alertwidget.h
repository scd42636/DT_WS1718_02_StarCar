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

public slots:

private:

    // Thread
    Alert               *alertThread;
    QStringListModel    *stringListModel;
    QStringList         *stringList;
    QListView           *listView;
    QVBoxLayout         *vBox1;
};

#endif // ALERTWIDGET_H
