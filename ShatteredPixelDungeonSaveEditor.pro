#-------------------------------------------------
#
# Project created by QtCreator 2019-04-07T17:53:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ShatteredPixelDungeonSaveEditor
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
		src\main.cpp \
		src\mainwindow.cpp \
		src\qjsonmodel.cpp \
    src/utils.cpp \
    src/newitemwidget.cpp

HEADERS += \
		src\mainwindow.h \
		src\qjsonmodel.h \
    src/utils.h \
    src/errors.h \
    src/newitemwidget.h

FORMS += \
		src\mainwindow.ui \
    src/newitemwidget.ui

INCLUDEPATH += libs\bit7z2\include

LIBS += -L$$PWD\libs\bit7z2\lib -lbit7z_d

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
