QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Sources/event.cpp \
    Sources/gate.cpp \
    Sources/main.cpp \
    Sources/mainwindow.cpp \
    Sources/net.cpp \
    Sources/netlist.cpp \
    Sources/simulation.cpp \
    Sources/timewheel.cpp

HEADERS += \
    Headers/event.h \
    Headers/gate.h \
    Headers/mainwindow.h \
    Headers/net.h \
    Headers/netlist.h \
    Headers/simulation.h \
    Headers/timewheel.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
