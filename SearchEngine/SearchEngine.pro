TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += \
    userinterface.cpp \
    searchenginecontrol.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    userinterface.h \
    searchenginecontrol.h

