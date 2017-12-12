#include "homewindow.h"
#include <QApplication>
#include <initstarcar.h>
#include <QThread>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HomeWindow w;

    w.show();
    //w.showFullScreen();
    //QApplication::setOverrideCursor(Qt::BlankCursor);
    return a.exec();
}
