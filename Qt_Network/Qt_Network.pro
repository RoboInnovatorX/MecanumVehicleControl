QT -= gui
QT += network
QT += serialport
QT += core
QT += xml

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

linux-beagle* {
    ROOTFS = $$(HOME)/Common-Installs/BBB/RootFS
    INCLUDEPATH += $${ROOTFS}/usr/include/arm-linux-gnueabihf
    LIBS += -lrobotcontrol
    LIBS += -lgpiod

SOURCES += \
        Qt_Server.cpp \
        XmlReadWrite.cpp \
        main.cpp
HEADERS += \
        Qt_Server.h \
        XmlReadWrite.h

}
message("Processor = " $$(Q_PROCESSOR_ARM))

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /home/debian/MyTests
!isEmpty(target.path): INSTALLS += target


