QT += core gui widgets multimedia

CONFIG += c++17

SOURCES += \
    GameLogic.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    GameLogic.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    map.qrc \
    resources.qrc

# Include paths and library paths
INCLUDEPATH += D:/Qt/6.8.0/mingw_64/include
LIBS += -LD:/Qt/6.8.0/mingw_64/lib

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
