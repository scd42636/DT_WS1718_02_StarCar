#include "alert.h"
#include "pathsandconstans.h"

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
            alert->setIcon(QIcon(errorImage));
            lastTimeOrange = false;

        }else{

            alert->setIcon(QIcon(warningImage));
            lastTimeOrange = true;
        }
    }else{


        if(warningIsSet && lastTimeWhite)
        {
            alert->setIcon(QIcon(warningImage));
            lastTimeWhite = false;

        }else if(errorIsSet && lastTimeWhite){

            alert->setIcon(QIcon(errorImage));

            lastTimeWhite = false;

        }else{

            alert->setIcon(QIcon(whiteWarningImage));
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
