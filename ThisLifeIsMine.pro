#-------------------------------------------------
#
# Project created by QtCreator 2020-09-12T10:56:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ThisLifeIsMine
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        authorization.cpp \
        budget.cpp \
        calendar.cpp \
        dairy.cpp \
        hobby.cpp \
        main.cpp \
        menu.cpp \
        notes.cpp \
        planner.cpp \
        timetable.cpp

HEADERS += \
        authorization.h \
        budget.h \
        calendar.h \
        dairy.h \
        hobby.h \
        menu.h \
        notes.h \
        planner.h \
        timetable.h

FORMS += \
        authorization.ui \
        budget.ui \
        calendar.ui \
        dairy.ui \
        hobby.ui \
        menu.ui \
        notes.ui \
        planner.ui \
        timetable.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
