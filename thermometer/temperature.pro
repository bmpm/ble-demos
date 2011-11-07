######################################################################
# Automatically generated by qmake (2.01a) Thu Jun 9 08:48:58 2011
######################################################################

QT += declarative
CONFIG += qdbus
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .

exists($$QMAKE_INCDIR_QT"/../qmsystem2/qmkeys.h"):!contains(MEEGO_EDITION,harmattan): {
  MEEGO_VERSION_MAJOR     = 1
  MEEGO_VERSION_MINOR     = 2
  MEEGO_VERSION_PATCH     = 0
  MEEGO_EDITION           = harmattan
  DEFINES += MEEGO_EDITION_HARMATTAN
}


# Input
SOURCES += main.cpp \
           manager.cpp \
           adapter.cpp \
           device.cpp \
           proximity.cpp \
           monitor.cpp \
           characteristic.cpp \

HEADERS += types.h \
           manager.h \
           adapter.h \
           device.h \
           proximity.h \
           monitor.h \
           characteristic.h \

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qml/MonitorView.qml \
    qml/MainMeego.qml \
    qml/Separator.qml \
    temperature.desktop \
    temperature.png \
    temperature.svg

RESOURCES += \
    resource.qrc

include(deployment.pri)
qtcAddDeployment()

unix:!symbian:!maemo5 {
    target.path = /opt/temperature/bin
    INSTALLS += target
}
