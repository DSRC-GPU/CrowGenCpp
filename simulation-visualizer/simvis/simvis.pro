#-------------------------------------------------
#
# Project created by QtCreator 2014-05-04T19:42:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simvis
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp\
        ../../crowdgeneration/CrowGenCpp/src/CrowdParser.cpp\
        ../../crowdgeneration/CrowGenCpp/src/Vertex.cpp\
        ../../crowdgeneration/CrowGenCpp/src/Point.cpp\
        ../../crowdgeneration/CrowGenCpp/src/tinyxml2-master/tinyxml2.cpp \
    graphmanager.cpp

HEADERS  += mainwindow.h \
    graphmanager.h

FORMS    += mainwindow.ui

