#-------------------------------------------------
#
# Project created by QtCreator 2025-07-19T15:56:11
#
#-------------------------------------------------

QT       += core gui
CONFIG+=resource_big
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Project
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    drawoptionswindow.cpp \
    filesystem.cpp \
    editdialog.cpp \
    managedocumentsdialog.cpp \
    groupmanagementdialog.cpp

HEADERS += \
        mainwindow.h \
    config.h \
    drawoptionswindow.h \
    filesystem.h \
    editdialog.h \
    managedocumentsdialog.h \
    groupmanagementdialog.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    resource.qrc

# ------ 使用下面的方式来集成 QXlsx ------
# 1. 告诉 qmake 在编译时包含 xlsx 模块
CONFIG += xlsx

# 2. 引入 QXlsx 的 .pri 文件，它会自动处理所有源文件和头文件的路径
include($$PWD/3rdparty/QXlsx/QXlsx.pri)
