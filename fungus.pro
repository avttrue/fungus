QT       += core gui widgets

CONFIG += c++17

TARGET       = fungus
TEMPLATE     = app
VERSION      = 1.0

DEFINES     += APP_VERS=\\\"$${VERSION}\\\"
DEFINES     += GIT_VERS=\\\"$$system(git describe --always)\\\"
#DEFINES     += GIT_VERS=\\\"-------\\\"
DEFINES     += BUILD_DATE='"\\\"$(shell date)\\\""'
DEFINES     += QT_DEPRECATED_WARNINGS

SOURCES += \
    dialogs/dialogcellinformation.cpp \
    dialogs/dialogeditrules.cpp \
    dialogs/dialogfieldinformation.cpp \
    dialogs/dialoginfocontent.cpp \
    dialogs/dialogvalueslist.cpp \
    dialogs/helpergraphics.cpp \
    dialogs/infopanel.cpp \
    field/cellinformation.cpp \
    field/cell.cpp \
    field/field.cpp \
    field/fieldinformation.cpp \
    field/fieldrule.cpp \
    kernel/kernel.cpp \
    scene/scene.cpp \
    scene/sceneitem.cpp \
    scene/sceneview.cpp \
    graphicsviewzoomer.cpp \
    config.cpp \
    controls.cpp \
    helper.cpp \
    jdocumentlist.cpp \
    main.cpp \
    mainwindow.cpp \
    properties.cpp \    
    splashscreen.cpp

HEADERS += \
    dialogs/dialogcellinformation.h \
    dialogs/dialogeditrules.h \
    dialogs/dialogfieldinformation.h \
    dialogs/dialoginfocontent.h \
    dialogs/dialogvalueslist.h \
    dialogs/helpergraphics.h \
    dialogs/infopanel.h \
    field/cellinformation.h \
    field/field.h \
    field/fieldinformation.h \
    field/fieldrule.h \
    field/cell.h \
    kernel/kernel.h \
    scene/scene.h \
    scene/sceneitem.h \
    scene/sceneview.h \
    graphicsviewzoomer.h \
    config.h \
    controls.h \
    helper.h \
    jdocumentlist.h \   
    mainwindow.h \
    properties.h \    
    splashscreen.h

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES +=
