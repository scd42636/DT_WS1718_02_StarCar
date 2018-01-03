//--------------------------------------------------------------------------------------------------
// <copyright file="UsbDriver.cpp" company="OTH Regensburg">
//     This file is protected by Team 02 StarCar Copyright (c) 2017.
// </copyright>
// <author>Dominik Scharnagl</author>
//--------------------------------------------------------------------------------------------------

#include "UsbDriver.h"


// ---------- Public constructors ----------

////UsbDriver::UsbDriver()
////    : CDCAsyncOper()
////{
////}

// ---------- Public methods ----------

uint8_t UsbDriver::OnInit(UsbController* controller)
{
    uint8_t rcode = 0;

    // Set DTR=1 and RTS=1:
    rcode = controller->SetControlLineState(3);

    if (rcode) {
        ErrorMessage<uint8_t>(PSTR("SetControlLineState"), rcode);
        return rcode;
    }

    LINE_CODING lc;
    lc.dwDTERate = 115200;
    lc.bCharFormat = 0;
    lc.bParityType = 0;
    lc.bDataBits = 8;

    rcode = controller->SetLineCoding(&lc);

    if (rcode)
        ErrorMessage<uint8_t>(PSTR("SetLineCoding"), rcode);

    return rcode;
}
