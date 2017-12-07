#include "alert.h"

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
            pButtonAlert->setIcon(QIcon("://Pics/alert_red.png"));
            lastTimeOrange = false;

        }else{

            pButtonAlert->setIcon(QIcon("://Pics/alert_orange.png"));
            lastTimeOrange = true;
        }
    }else{


        if(warningIsSet && lastTimeWhite)
        {
            pButtonAlert->setIcon(QIcon("://Pics/alert_orange.png"));
            lastTimeWhite = false;

        }else if(errorIsSet && lastTimeWhite){

            pButtonAlert->setIcon(QIcon("://Pics/alert_red.png"));

            lastTimeWhite = false;

        }else{

            pButtonAlert->setIcon(QIcon("://Pics/alert_white.png"));
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

    this->warMsg.insert(this->warMsg.end(),warMsg);
}

void Alert::fireWarning(QString warMsg, int warNum){

    if(warningIsSet == false){

        warningIsSet = true;

        if(errorIsSet == true){

            warningAndErrorIsSet = true;
        }
    }

    this->warMsg.insert(this->warMsg.end(),"Nr: " + QString::number(warNum) + "     " + warMsg);
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

    this->errMsg.insert(this->errMsg.end(),errMsg);
}

void Alert::fireError(QString errMsg, int errNum){

    if(errorIsSet == false){

        errorIsSet = true;

        if(warningIsSet){

            warningAndErrorIsSet = true;
        }
    }

    this->errMsg.insert(this->errMsg.end(),"Nr: " + QString::number(errNum) + "     " + errMsg);
}

void Alert::clearWarning(){

    warningIsSet = false;
    warningAndErrorIsSet = false;
}

void Alert::clearError(){

    errorIsSet = false;
    warningAndErrorIsSet = false;
}

QVector<QString> Alert::getwarMsg(){

    return warMsg;
}

QVector<QString> Alert::geterrMsg(){

    return errMsg;
}

void Alert::finishAlert(){

    emit finished();
}
