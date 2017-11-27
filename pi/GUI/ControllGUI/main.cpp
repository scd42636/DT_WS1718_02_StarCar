#include "homewindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HomeWindow w;
    w.show();

    //w.showFullScreen();
    return a.exec();
}
