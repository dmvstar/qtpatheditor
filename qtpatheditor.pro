#-------------------------------------------------
#
# Project created by QtCreator 2014-05-28T12:50:48
#
#-------------------------------------------------

QT       += core gui winextras

LIBS += -ladvapi32 #-lshell32 -lkernel32 -luser32.lib  gdi32.lib shell32.lib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtpatheditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cpathreader.cpp \
    utils.cpp

HEADERS  += mainwindow.h \
    cpathreader.h \
    utils.h

FORMS    += mainwindow.ui

RESOURCES += \
    res/icons.qrc \
    lang/i18n.qrc

TRANSLATIONS += lang/qtpatheditor_ru.ts

