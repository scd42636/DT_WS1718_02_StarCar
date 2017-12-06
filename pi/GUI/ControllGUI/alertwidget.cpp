#include "alertwidget.h"



AlertWidget::AlertWidget(QWidget *parent, Alert *alertThread) : QWidget(parent)
{
    this->alertThread = alertThread;

    vBox1 = new QVBoxLayout(this);

    vBox1->setContentsMargins(0,0,0,0);

    listView = new QListView();
    vBox1->addWidget(listView);
    listView->setContentsMargins(0,0,0,0);

    stringListModel = new QStringListModel(this);
    stringList = new QStringList();
    QStringList test;

    test << "test" << "test2";

    //stringList->join("test");

    stringListModel->setStringList(test);
    listView->setModel(stringListModel);

}



AlertWidget::~AlertWidget(){

}
