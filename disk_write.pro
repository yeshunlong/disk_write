QT += core gui widgets

CONFIG += c++17

win32-msvc*:QMAKE_CXXFLAGS += /utf-8
# QMAKE_LFLAGS += "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\""

FORMS += $$PWD/src/disk_write.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    build/ui_disk_write.h \
    src/disk_write.h \
    src/multi_process.h \
    src/write_func.h

SOURCES += \
    src/disk_write.cpp \
    src/main.cpp
