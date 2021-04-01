QT       += core gui widgets
#https://stackoverflow.com/a/53234995/12177714
QMAKE_CXXFLAGS += "-fno-sized-deallocation"

CONFIG += c++17

TARGET       = fungus
TEMPLATE     = app

DEFINES     += GIT_VERS=\\\"$$system(git describe --always)\\\"
#DEFINES     += GIT_VERS=\\\"-------\\\"
DEFINES     += BUILD_DATE='"\\\"$(shell date)\\\""'
DEFINES     += QT_DEPRECATED_WARNINGS
DEFINES     += QT_MESSAGELOGCONTEXT

SOURCES += \
    controls/clickablelabel.cpp \
    controls/infopanel.cpp \
    controls/menucaption.cpp \
    controls/separators.cpp \
    controls/snapshotlist.cpp \
    controls/textlog.cpp \
    dialogs/dialogbody.cpp \
    dialogs/dialogcellinformation.cpp \
    dialogs/dialogcellmonitor.cpp \
    dialogs/dialogeditrules.cpp \
    dialogs/dialogfieldinformation.cpp \
    dialogs/dialoginfocontent.cpp \
    dialogs/dialogvalueslist.cpp \
    dialogs/helpergraphics.cpp \
    field/cellinformation.cpp \
    field/cell.cpp \
    field/field.cpp \
    field/fieldinformation.cpp \
    field/fieldrule.cpp \
    helpers/helper.cpp \
    helpers/tooltipshortcut.cpp \
    helpers/widgethelper.cpp \
    kernel/kernel.cpp \
    scene/graphicsviewzoomer.cpp \
    scene/scene.cpp \
    scene/sceneitem.cpp \
    scene/sceneview.cpp \
    config.cpp \
    main.cpp \
    mainwindow.cpp \
    properties.cpp \
    splashscreen.cpp

HEADERS += \
    controls/clickablelabel.h \
    controls/infopanel.h \
    controls/menucaption.h \
    controls/separators.h \
    controls/snapshotlist.h \
    controls/textlog.h \
    dialogs/dialogbody.h \
    dialogs/dialogcellinformation.h \
    dialogs/dialogcellmonitor.h \
    dialogs/dialogeditrules.h \
    dialogs/dialogfieldinformation.h \
    dialogs/dialoginfocontent.h \
    dialogs/dialogvalueslist.h \
    dialogs/helpergraphics.h \
    field/cellinformation.h \
    field/field.h \
    field/fieldinformation.h \
    field/fieldrule.h \
    field/cell.h \
    helpers/helper.h \
    helpers/tooltipshortcut.h \
    helpers/widgethelper.h \
    kernel/kernel.h \
    scene/graphicsviewzoomer.h \
    scene/scene.h \
    scene/sceneitem.h \
    scene/sceneview.h \
    config.h \
    mainwindow.h \
    properties.h \    
    splashscreen.h

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES +=
