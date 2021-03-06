QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += static

RC_ICONS = icon.ico
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    previewdialog.cpp \
    refdialog.cpp \
    settingdialog.cpp \
    setupdialog.cpp

HEADERS += \
    aboutdialog.h \
    mainwindow.h \
    previewdialog.h \
    refdialog.h \
    settingdialog.h \
    setupdialog.h

FORMS += \
    aboutdialog.ui \
    mainwindow.ui \
    previewdialog.ui \
    refdialog.ui \
    settingdialog.ui \
    setupdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
