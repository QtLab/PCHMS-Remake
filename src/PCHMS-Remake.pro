#-------------------------------------------------
#
# Project created by QtCreator 2012-09-14T16:02:46
#
#-------------------------------------------------

QT       += core gui

TARGET = PCHMS-Remake
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    keyboard.cpp \
    hook.cpp \
    statistic.cpp \
    mouse.cpp \
    recorder.cpp \
    tray.cpp \
    colormanagement.cpp \
    algorithm.cpp \
    inadaywidget.cpp \
    chooseprogramlist.cpp \
    inadayoperationchart.cpp \
    timelinechart.cpp \
    inadaytimechart.cpp \
    mousespectrogram.cpp \
    suggestionitem.cpp \
    linechart.cpp \
    linechartpicker.cpp \
    habitdial.cpp \
    pieview.cpp \
    activeperiodchart.cpp \
    histogram.cpp \
    globalchart.cpp \
    keyboardspectrogram.cpp \
    programwidget.cpp \
    aboutauthordialog.cpp \
    aboutprojectdialog.cpp \
    operationlistview.cpp \
    suggestion.cpp \
    suggestionview.cpp \
    topwidget.cpp

HEADERS  += mainwindow.h \
    win32systemcall.h \
    keyboard.h \
    hook.h \
    statistic.h \
    mouse.h \
    recorder.h \
    tray.h \
    operationinaday.h \
    colormanagement.h \
    algorithm.h \
    inadaywidget.h \
    chooseprogramlist.h \
    inadayoperationchart.h \
    timelinechart.h \
    inadaytimechart.h \
    mousespectrogram.h \
    suggestionitem.h \
    linechart.h \
    linechartpicker.h \
    habitdial.h \
    pieview.h \
    activeperiodchart.h \
    histogram.h \
    globalchart.h \
    keyboardspectrogram.h \
    programwidget.h \
    aboutauthordialog.h \
    aboutprojectdialog.h \
    operationlistview.h \
    suggestion.h \
    suggestionview.h \
    topwidget.h

FORMS    += mainwindow.ui \
    inadaywidget.ui \
    programwidget.ui \
    aboutauthordialog.ui \
    aboutprojectdialog.ui

CONFIG += qwt
