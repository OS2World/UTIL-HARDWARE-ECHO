TEMPLATE=app

HEADERS += mainwindow.h PCITableModel.h
SOURCES += main.cpp
SOURCES += mainwindow.cpp PCITableModel.cpp
FORMS += mainwindow.ui about.ui
LIBS += -lz -lpci -lresolv
RESOURCES += echo.qrc
QMAKE_EXTRA_TARGETS += devgz

QT += network
CONFIG += qt windows debug_and_release
CONFIG(debug, debug|release) {
  TARGET = echo_d
} else {
  TARGET = echo
}

win32 {
  INCLUDEPATH += .. ..\zlib
}
#unix:QMAKE_LFLAGS += -Wl,-static


devgz.target = data/devices.lst.gz
devgz.commands = gzip < data/DEVICE.LST > $$devgz.target
devgz.depends = data/DEVICE.LST


