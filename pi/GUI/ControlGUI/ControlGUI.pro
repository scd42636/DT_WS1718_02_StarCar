#-------------------------------------------------
#
# Project created by QtCreator 2017-11-16T18:39:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ControlGUI
TEMPLATE = app
CONFIG += c++14
!win32{
LIBS+=-lurg_c
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


win32{

SOURCES += main.cpp\
    homewindow.cpp \
    startwidget.cpp \
    alert.cpp \
    alertwidget.cpp \
    exitwidget.cpp \
    operationmodewidget.cpp \
    initstarcar.cpp \
    clockcontrolmodewidget.cpp \
    clockcontrollmode.cpp \
    controllercontrolmodewidget.cpp \
    sensorvalueswidget.cpp \

HEADERS += homewindow.h \
    startwidget.h \
    alert.h \
    alertwidget.h \
    exitwidget.h \
    operationmodewidget.h \
    initstarcar.h \
    clockcontrolmodewidget.h \
    clockcontrolmode.h \
    controllercontrolmodewidget.h \
    sensorvalueswidget.h \
}

!win32{

SOURCES += main.cpp\
    homewindow.cpp \
    startwidget.cpp \
    alert.cpp \
    alertwidget.cpp \
    exitwidget.cpp \
    operationmodewidget.cpp \
    initstarcar.cpp \
    clockcontrolmodewidget.cpp \
    clockcontrollmode.cpp \
    controllercontrolmodewidget.cpp \
    sensorvalueswidget.cpp \
    threadlidar.cpp \
    ../../IBP/IBC.cpp \
    ../../IBP/IBC_Inbox.cpp \
    ../../IBP/IBC_Packet.cpp \
    ../../IBP/IBC_Rule.cpp \
    ../../IBP/IBC_Transceiver.cpp \
    ../../IBP/Serial.cpp \
    ../../lidar/lidar.cpp

HEADERS += homewindow.h \
    startwidget.h \
    alert.h \
    alertwidget.h \
    exitwidget.h \
    operationmodewidget.h \
    initstarcar.h \
    clockcontrolmodewidget.h \
    clockcontrolmode.h \
    controllercontrolmodewidget.h \
    sensorvalueswidget.h \
    threadlidar.h \
    ../../IBP/IBC.hpp \
    ../../IBP/IBC_Inbox.hpp \
    ../../IBP/IBC_Packet.hpp \
    ../../IBP/IBC_Rule.hpp \
    ../../IBP/IBC_Transceiver.hpp \
    ../../IBP/Serial.hpp \
    ../../lidar/lidar.h \
    ../../lidar/include/urg_connection.h \
    ../../lidar/include/urg_debug.h \
    ../../lidar/include/urg_detect_os.h \
    ../../lidar/include/urg_errno.h \
    ../../lidar/include/urg_ring_buffer.h \
    ../../lidar/include/urg_sensor.h \
    ../../lidar/include/urg_serial.h \
    ../../lidar/include/urg_serial_utils.h \
    ../../lidar/include/urg_tcpclient.h \
    ../../lidar/include/urg_utils.h
}

FORMS += homewindow.ui

DISTFILES += \
    ../../IBP/IBC_config.cfg \

RESOURCES += \
    paths.qrc

HEADERS += \
    ../SerialProtocol/SerialProtocol.h \
    ../SerialProtocol/StreamSerialProtocol.h \
    ../SerialProtocol/starcarprotocol.h

SOURCES += \
    ../SerialProtocol/SerialProtocol.cpp \
    ../SerialProtocol/StreamSerialProtocol.cpp \
    ../SerialProtocol/starcarprotocol.cpp
