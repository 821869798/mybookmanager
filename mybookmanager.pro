#-------------------------------------------------
#
# Project created by QtCreator 2015-09-09T21:51:45
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mybookmanager
TEMPLATE = app


SOURCES += main.cpp\
    logindialog.cpp \
    tool.cpp \
    readerdialog.cpp \
    usrinformation.cpp \
    borrowhistory.cpp \
    admindialog.cpp \
    addbookdialog.cpp \
    addreaderdialog.cpp \
    tempquerydialog.cpp

HEADERS  += \
    logindialog.h \
    tool.h \
    readerdialog.h \
    usrinformation.h \
    borrowhistory.h \
    admindialog.h \
    addbookdialog.h \
    addreaderdialog.h \
    tempquerydialog.h

FORMS    += \
    logindialog.ui \
    readerdialog.ui \
    borrowhistory.ui \
    admindialog.ui \
    addbookdialog.ui \
    addreaderdialog.ui \
    tempquerydialog.ui
