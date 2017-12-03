#include "alert.h"
#include "pathsandconstans.h"

Alert::Alert(QPushButton *alert) {

    this->pButtonAlert = alert;
}

Alert::~Alert() {

}

void Alert::process() {

}

void Alert::changeAlertIcon(){

    if(warningAndErrorIsSet){

        if(lastTimeOrange)
        {
            pButtonAlert->setIcon(QIcon(errorImage));
            lastTimeOrange = false;

        }else{

            pButtonAlert->setIcon(QIcon(warningImage));
            lastTimeOrange = true;
        }
    }else{


        if(warningIsSet && lastTimeWhite)
        {
            pButtonAlert->setIcon(QIcon(warningImage));
            lastTimeWhite = false;

        }else if(errorIsSet && lastTimeWhite){

            pButtonAlert->setIcon(QIcon(errorImage));

            lastTimeWhite = false;

        }else{

            pButtonAlert->setIcon(QIcon(whiteWarningImage));
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

void Alert::fireWarning(QString warMsg){

    if(warningIsSet == false){

        warningIsSet = true;

        if(errorIsSet == true){

            warningAndErrorIsSet = true;
        }
    }

    this->warMsg.append(warMsg);
}

void Alert::fireWarning(QString warMsg, int warNum){

    if(warningIsSet == false){

        warningIsSet = true;

        if(errorIsSet == true){

            warningAndErrorIsSet = true;
        }
    }

    this->warMsg.append(warMsg);
    this->warNum[warNumCount] = warNum;
    warNumCount++;
    qDebug("%d",warNumCount);
}

void Alert::fireError(){

    if(errorIsSet == false){

        errorIsSet = true;

        if(warningIsSet){

            warningAndErrorIsSet = true;
        }
    }
}

void Alert::fireError(QString errMsg){

    if(errorIsSet == false){

        errorIsSet = true;

        if(warningIsSet){

            warningAndErrorIsSet = true;
        }
    }

    this->errMsg.append(errMsg);
}

void Alert::fireError(QString errMsg, int errNum){

    if(errorIsSet == false){

        errorIsSet = true;

        if(warningIsSet){

            warningAndErrorIsSet = true;
        }
    }

    this->errMsg.append(errMsg);
    this->errNum[errNumCount] = errNum;
    errNumCount++;
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
