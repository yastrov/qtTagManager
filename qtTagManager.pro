#-------------------------------------------------
#
# Project created by QtCreator 2017-05-04T20:34:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtTagManager
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

DEFINES += QT_USE_QSTRINGBUILDER
CONFIG += c++11

CONFIG(release, debug|release): {
    DEFINES += QT_NO_DEBUG_OUTPUT
} else {
    DEFINES += MYPREFIX_DEBUG
}

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/tagitem.cpp \
    src/tagtreemodel.cpp \
    src/colorhelper/tagcolorhelper.cpp \
    src/colorhelper/tagcolorhelperdefined.cpp \
    src/validator/tagvalidcheckhelper.cpp \
    src/store/abstracttagstorehelper.cpp \
    src/store/tagstorehelperxml.cpp \
    src/settingshelper.cpp

HEADERS  += src/mainwindow.h \
    src/tagitem.h \
    src/tagtreemodel.h \
    src/colorhelper/tagcolorhelper.h \
    src/colorhelper/tagcolorhelperdefined.h \
    src/abstracttagcolorhelper.h \
    src/validator/abstracttagvalidcheckhelper.h \
    src/validator/tagvalidcheckhelper.h \
    src/store/abstracttagstorehelper.h \
    src/store/tagstorehelperxml.h \
    src/colorhelper/abstracttagcolorhelper.h \
    src/settingshelper.h


FORMS    += src/mainwindow.ui

TRANSLATIONS += translations/qtagmanager_ru.ts

VERSION = 0.0.0.1
QMAKE_TARGET_COMPANY = OpenSource
QMAKE_TARGET_PRODUCT = qtTagManager
QMAKE_TARGET_DESCRIPTION = "Tag manager"
QMAKE_TARGET_COPYRIGHT = (c) Yuriy Astrov
# RC_ICONS = myappico.ico
# RC_FILE = myapp.rc
GITHUB_URL = "https://github.com/yastrov/qtTagManager"

DEFINES += \
APP_VERSION=\"\\\"$$VERSION\\\"\" \
APP_URL=\"\\\"$$GITHUB_URL\\\"\" \
QMAKE_TARGET_PRODUCT=\"\\\"$$QMAKE_TARGET_PRODUCT\\\"\" \
QMAKE_TARGET_COMPANY=\"\\\"$$QMAKE_TARGET_COMPANY\\\"\" \
