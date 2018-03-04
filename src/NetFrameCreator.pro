#-------------------------------------------------
#
# Project created by QtCreator 2016-06-10T22:43:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += network

TARGET = NetFrameCreator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    statusinfo.cpp \
    creator.cpp \
    ipv4header.cpp \
    arpheader.cpp \
    tcpheader.cpp \
    udpheader.cpp \
    icmpheader.cpp \
    statusinfocontrol.cpp \
    infodisplay.cpp \
    netcard.cpp \
    macheader.cpp \
    customdata.cpp \
    recvmessage.cpp \
    sendpacket.cpp \
    tools.cpp

HEADERS  += mainwindow.h \
    statusinfo.h \
    creator.h \
    ipv4header.h \
    arpheader.h \
    tcpheader.h \
    udpheader.h \
    icmpheader.h \
    statusinfocontrol.h \
    infodisplay.h \
    netcard.h \
    macheader.h \
    customdata.h \
    recvmessage.h \
    sendpacket.h \
    tools.h

FORMS    += mainwindow.ui \
    statusinfo.ui \
    creator.ui \
    ipv4header.ui \
    arpheader.ui \
    tcpheader.ui \
    udpheader.ui \
    icmpheader.ui \
    infodisplay.ui \
    macheader.ui \
    customdata.ui \
    tools.ui

RESOURCES += \
    pic.qrc
