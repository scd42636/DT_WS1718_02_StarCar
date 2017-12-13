#include "homewindow.h"
#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HomeWindow w;

#ifdef WIN32

    w.show();
    qDebug("Windows");

#elif Q_OS_MAC

    w.show();
    qDebug("Mac");
#else

    w.showFullScreen();
    //QApplication::setOverrideCursor(Qt::BlankCursor);
#endif
    return a.exec();
}
