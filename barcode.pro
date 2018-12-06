#-------------------------------------------------
#
# Project created by QtCreator 2018-11-05T17:14:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = codeGenerator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += \
        QT_DEPRECATED_WARNINGS \
        NO_PNG \
        NO_QR

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG +=  c++11


INCLUDEPATH += zint/backend


SOURCES += \
        zint/backend/2of5.c \
        zint/backend/auspost.c \
        zint/backend/aztec.c \
        zint/backend/bmp.c \
        zint/backend/codablock.c \
        zint/backend/code.c \
        zint/backend/code128.c \
        zint/backend/code16k.c \
        zint/backend/code49.c \
        zint/backend/common.c \
        zint/backend/composite.c \
        zint/backend/dmatrix.c \
        zint/backend/dotcode.c \
        zint/backend/eci.c \
        zint/backend/emf.c \
        zint/backend/gif.c \
        zint/backend/gridmtx.c \
        zint/backend/gs1.c \
        zint/backend/hanxin.c \
        zint/backend/imail.c \
        zint/backend/large.c \
        zint/backend/library.c \
        zint/backend/mailmark.c \
        zint/backend/maxicode.c \
        zint/backend/medical.c \
        zint/backend/pcx.c \
        zint/backend/pdf417.c \
        zint/backend/plessey.c \
        zint/backend/postal.c \
        zint/backend/ps.c \
        zint/backend/raster.c \
        zint/backend/reedsol.c \
        zint/backend/render.c \
        zint/backend/rss.c \
        zint/backend/svg.c \
        zint/backend/telepen.c \
        zint/backend/tif.c \
        zint/backend/upcean.c \
        zint/backend/qr.c \
        zint/backend/dllversion.c \
        zint/backend/code1.c \
        zint/backend/png.c \
        main.cpp \
        mainwindow.cpp \
        config.cpp \
        codeform.cpp

HEADERS += \
        zint/backend/aztec.h \
        zint/backend/bmp.h \
        zint/backend/code49.h \
        zint/backend/common.h \
        zint/backend/composite.h \
        zint/backend/dmatrix.h \
        zint/backend/eci.h \
        zint/backend/font.h \
        zint/backend/gridmtx.h \
        zint/backend/gs1.h \
        zint/backend/hanxin.h \
        zint/backend/large.h \
        zint/backend/maxicode.h \
        zint/backend/pcx.h \
        zint/backend/pdf417.h \
        zint/backend/reedsol.h \
        zint/backend/rss.h \
        zint/backend/sjis.h \
        zint/backend/stdint_msvc.h \
        zint/backend/zint.h \
        zint/backend_qt/qzint.h \
        mainwindow.h \
        config.h \
        codeform.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RC_FILE = barcode.rc

RESOURCES += \
    resource.qrc
