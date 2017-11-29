#include "alert.h"

Alert::Alert(QPushButton *alert) {

    this->alert = alert;
}

Alert::~Alert() {

}

void Alert::process() {

}

void Alert::changeAlertIcon(){

    if(warningAndErrorIsSet){

        if(lastTimeOrange)
        {
            alert->setIcon(QIcon("C:/Users/Flo/Desktop/DT_WS1718_02_StarCar/pi/GUI/ControllGUI/Pics/alert_red.png"));
            lastTimeOrange = false;

        }else{

            alert->setIcon(QIcon("C:/Users/Flo/Desktop/DT_WS1718_02_StarCar/pi/GUI/ControllGUI/Pics/alert_orange.png"));
            lastTimeOrange = true;
        }
    }else{


        if(warningIsSet && lastTimeWhite)
        {
            alert->setIcon(QIcon("C:/Users/Flo/Desktop/DT_WS1718_02_StarCar/pi/GUI/ControllGUI/Pics/alert_orange.png"));
            lastTimeWhite = false;

        }else if(errorIsSet && lastTimeWhite){

            alert->setIcon(QIcon("C:/Users/Flo/Desktop/DT_WS1718_02_StarCar/pi/GUI/ControllGUI/Pics/alert_red.png"));

            lastTimeWhite = false;

        }else{

            alert->setIcon(QIcon("C:/Users/Flo/Desktop/DT_WS1718_02_StarCar/pi/GUI/ControllGUI/Pics/alert_white.png"));
            lastTimeWhite = true;
        }
    }
}

void Alert::fireWarning(){

    if(warningIsSet == false){

        warningIsSet = true;

        if(errorIsSet == true){

            warningAndErrorIsSet = true;
        }
    }
}

void Alert::fireError(){

    if(errorIsSet == false){

        errorIsSet = true;

        if(warningIsSet){

            warningAndErrorIsSet = true;
        }
    }
}

void Alert::clearWarning(){

    warningIsSet = false;
    warningAndErrorIsSet = false;
}

void Alert::clearError(){

    errorIsSet = false;
    warningAndErrorIsSet = false;
}

void Alert::finishWorker(){

    emit finished();
}
