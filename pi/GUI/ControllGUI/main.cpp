#include "homewindow.h"
#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HomeWindow w;

#ifdef WIN32
    w.show();
#else
    w.showFullScreen();
    QApplication::setOverrideCursor(Qt::BlankCursor);
#endif
    return a.exec();
}
