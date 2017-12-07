/*
 * <div>Icons made by <a href="http://www.freepik.com" title="Freepik">Freepik</a> from <a href="https://www.flaticon.com/" title="Flaticon">www.flaticon.com</a> is licensed by <a href="http://creativecommons.org/licenses/by/3.0/" title="Creative Commons BY 3.0" target="_blank">CC 3.0 BY</a></div>
 * <div>Icons made by <a href="https://www.flaticon.com/authors/dave-gandy" title="Dave Gandy">Dave Gandy</a> from <a href="https://www.flaticon.com/" title="Flaticon">www.flaticon.com</a> is licensed by <a href="http://creativecommons.org/licenses/by/3.0/" title="Creative Commons BY 3.0" target="_blank">CC 3.0 BY</a></div>
 *
 * */

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
    return a.exec();
}
