QT       += core gui widgets

CONFIG += c++17

TARGET      = fungus
TEMPLATE    = app
VERSION     = 0.1

DEFINES     += APP_VERS=\\\"$${VERSION}\\\"
DEFINES     += GIT_VERS=\\\"$$system(git describe --always)\\\"
#DEFINES     += GIT_VERS=\\\"-------\\\"
DEFINES     += BUILD_DATE='"\\\"$(shell date)\\\""'
DEFINES     += QT_DEPRECATED_WARNINGS

SOURCES += \
    dialogs/dialogcellinformation.cpp \
    dialogs/dialogfieldinformation.cpp \
    dialogs/dialoginfopanel.cpp \
    dialogs/dialogvalueslist.cpp \
    dialogs/helpergraphics.cpp \
    field/cellinformation.cpp \
    field/field.cpp \
    field/fieldinformation.cpp \
    field/fieldrule.cpp \
    graphicsviewzoomer.cpp \
    config.cpp \
    controls.cpp \
    helper.cpp \
    kernel/kernel.cpp \
    main.cpp \
    mainwindow.cpp \
    properties.cpp \
    field/cell.cpp \
    scene/scene.cpp \
    scene/sceneitem.cpp \
    scene/sceneview.cpp

HEADERS += \
    dialogs/dialogcellinformation.h \
    dialogs/dialogfieldinformation.h \
    dialogs/dialoginfopanel.h \
    dialogs/dialogvalueslist.h \
    dialogs/helpergraphics.h \
    field/cellinformation.h \
    field/field.h \
    field/fieldinformation.h \
    field/fieldrule.h \
    graphicsviewzoomer.h \
    config.h \
    controls.h \
    helper.h \
    kernel/kernel.h \
    mainwindow.h \
    properties.h \
    field/cell.h \
    scene/scene.h \
    scene/sceneitem.h \
    scene/sceneview.h \
    wait.h

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
