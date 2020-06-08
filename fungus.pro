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
    dialogs/dialogvalueslist.cpp \
    dialogs/helpergraphics.cpp \
    field/cellinformation.cpp \
    field/cellrule.cpp \
    field/field.cpp \
    field/fieldservice.cpp \
    graphicsviewzoomer.cpp \
    config.cpp \
    controls.cpp \
    helper.cpp \
    main.cpp \
    mainwindow.cpp \
    properties.cpp \
    field/cell.cpp \
    scene/scene.cpp \
    scene/sceneobject.cpp \
    scene/sceneview.cpp

HEADERS += \
    dialogs/dialogcellinformation.h \
    dialogs/dialogvalueslist.h \
    dialogs/helpergraphics.h \
    field/cellinformation.h \
    field/cellrule.h \
    field/field.h \
    field/fieldservice.h \
    graphicsviewzoomer.h \
    config.h \
    controls.h \
    helper.h \
    mainwindow.h \
    properties.h \
    field/cell.h \
    scene/sceneobject.h \
    scene/scene.h \
    scene/sceneview.h

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
