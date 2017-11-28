#include "alert.h"

Alert::Alert(QPushButton *alert) { // Constructor
    // you could copy data from constructor arguments to internal variables here.
    this->alert = alert;
}

Alert::~Alert() { // Destructor
    // free resources
}

void Alert::process() { // Process. Start processing data.

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(changeAlertIcon()));
    timer->start(300);
}

void Alert::changeAlertIcon(){

    if(red == 0){
        alert->setIcon(QIcon("C:/Users/Flo/Desktop/DT_WS1718_02_StarCar/pi/GUI/ControllGUI/Pics/alert_white.png"));
        red = 1;
    }else if(red == 1){
        alert->setIcon(QIcon("C:/Users/Flo/Desktop/DT_WS1718_02_StarCar/pi/GUI/ControllGUI/Pics/alert_red.png"));
        red = 0;
    }else{
        alert->setIcon(QIcon("C:/Users/Flo/Desktop/DT_WS1718_02_StarCar/pi/GUI/ControllGUI/Pics/alert_orange.png"));
        red = 0;
    }
}

void Alert::finishWorker(){


    emit finished();
}
