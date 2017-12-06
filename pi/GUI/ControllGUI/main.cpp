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

   /* QThread *thread = new QThread;
    InitStarCar *initStarCar = new InitStarCar(nullptr);
    InitStarCar->moveToThread(thread);

    connect(thread, SIGNAL(started()), initStarCar, SLOT(startProcess()));
    connect(initStarCar, SIGNAL(finished()), thread, SLOT(quit()));
    connect(initStarCar, SIGNAL(finished()), initStarCar, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
*/
    //w.showFullScreen();
    return a.exec();
}
