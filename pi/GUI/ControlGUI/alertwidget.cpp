#include "alertwidget.h"
#include <QVector>

bool warIsEmpty = false;
bool errIsEmpty = false;

AlertWidget::AlertWidget(QWidget *parent, Alert *alertThread) : QWidget(parent)
{
    this->alertThread = alertThread;

    if(alertThread->getwarMsg().isEmpty()){

        warIsEmpty = true;

    }else{

        warIsEmpty = false;
    }

    if(alertThread->geterrMsg().isEmpty()){

        errIsEmpty = true;

    }else{

        errIsEmpty = false;
    }

    generateLayout();
    generateStyle();
    setupConnects();
}

void AlertWidget::generateLayout(){

    vBox1           = new QVBoxLayout(this);
    checkWarErr     = new QPushButton();

    listView        = new QListView();
    stringListModel = new QStringListModel(this);
    stringList      = new QStringList();

    vBox1->addWidget(listView);
    vBox1->addWidget(checkWarErr);

    QStringList data;

    if(warIsEmpty && errIsEmpty)
    {
        data << "Keine Meldungen!";

    }

    if(warIsEmpty == false){

        data << "Warnungen:\n";

        for (QString warMsg : alertThread->getwarMsg()){

            data << warMsg;
        }
    }

    if(errIsEmpty == false){

        data << "\nFehler: \n";

        for (QString errMsg : alertThread->geterrMsg()){

            data << errMsg;
        }
    }

    stringListModel->setStringList(data);
    listView->setModel(stringListModel);
}

void AlertWidget::generateStyle(){

    vBox1->setContentsMargins(0,0,0,0);
    listView->setContentsMargins(0,0,0,0);

    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listView->setSelectionMode(QAbstractItemView::NoSelection);
    listView->setFocusPolicy(Qt::NoFocus);

    listView->setStyleSheet("QListView{"
                                "border: 0px}"
                            "QListView:item{"
                                "font-style: normal;"
                                "font-size: 9pt;"
                                "font-weight: bold;"
                                "font-family: TimesNewRoman;"
                                "color: white}");

    checkWarErr->setStyleSheet("QPushButton{"
                            "color: green;"
                            "font-family: TimesNewRoman;"
                            "font-style: normal;"
                            "font-size: 10pt;"
                            "font-weight: bold;}");
    QFont test = QFont("TimesNewRoman",9);
    test.setBold(true);
    listView->setFont(test);

    checkWarErr->setText("Quittieren");
}

void AlertWidget::setupConnects(){

    connect(checkWarErr, SIGNAL(clicked(bool)), alertThread, SLOT(clearWarning()));
    connect(checkWarErr, SIGNAL(clicked(bool)), alertThread, SLOT(clearError()));
}

AlertWidget::~AlertWidget(){

}
