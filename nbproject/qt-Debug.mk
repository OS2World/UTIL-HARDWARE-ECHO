#############################################################################
# Makefile for building: dist/Debug/GNU-Linux-x86/echo
# Generated by qmake (2.01a) (Qt 4.6.2) on: Thu Mar 31 16:03:09 2011
# Project:  nbproject/qt-Debug.pro
# Template: app
# Command: /usr/bin/qmake -unix VPATH=. -o qttmp-Debug.mk nbproject/qt-Debug.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/local/Trolltech/Qt-4.6.2/mkspecs/linux-g++ -Inbproject -I/usr/local/Trolltech/Qt-4.6.2/include/QtCore -I/usr/local/Trolltech/Qt-4.6.2/include/QtGui -I/usr/local/Trolltech/Qt-4.6.2/include -I. -I. -Inbproject -I.
LINK          = g++
LFLAGS        = -Wl,-rpath,/usr/local/Trolltech/Qt-4.6.2/lib
LIBS          = $(SUBLIBS)  -L/usr/local/Trolltech/Qt-4.6.2/lib -lpci -lz -lresolv -lQtGui -L/usr/local/Trolltech/Qt-4.6.2/lib -L/usr/X11R6/lib -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = build/Debug/GNU-Linux-x86/

####### Files

SOURCES       = mainwindow.cpp \
		main.cpp \
		PCITableModel.cpp moc_mainwindow.cpp \
		qrc_echo.cpp
OBJECTS       = build/Debug/GNU-Linux-x86/mainwindow.o \
		build/Debug/GNU-Linux-x86/main.o \
		build/Debug/GNU-Linux-x86/PCITableModel.o \
		build/Debug/GNU-Linux-x86/moc_mainwindow.o \
		build/Debug/GNU-Linux-x86/qrc_echo.o
DIST          = /usr/local/Trolltech/Qt-4.6.2/mkspecs/common/g++.conf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/common/unix.conf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/common/linux.conf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/qconfig.pri \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/qt_functions.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/qt_config.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/exclusive_builds.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/default_pre.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/debug.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/default_post.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/warn_on.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/qt.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/unix/thread.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/moc.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/resources.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/uic.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/yacc.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/lex.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/include_source_dir.prf \
		nbproject/qt-Debug.pro
QMAKE_TARGET  = echo
DESTDIR       = dist/Debug/GNU-Linux-x86/
TARGET        = dist/Debug/GNU-Linux-x86/echo

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: qttmp-Debug.mk $(TARGET)

$(TARGET): ui_about.h ui_mainwindow.h $(OBJECTS)  
	@$(CHK_DIR_EXISTS) dist/Debug/GNU-Linux-x86/ || $(MKDIR) dist/Debug/GNU-Linux-x86/ 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

qttmp-Debug.mk: nbproject/qt-Debug.pro  /usr/local/Trolltech/Qt-4.6.2/mkspecs/linux-g++/qmake.conf /usr/local/Trolltech/Qt-4.6.2/mkspecs/common/g++.conf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/common/unix.conf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/common/linux.conf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/qconfig.pri \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/qt_functions.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/qt_config.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/exclusive_builds.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/default_pre.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/debug.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/default_post.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/warn_on.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/qt.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/unix/thread.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/moc.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/resources.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/uic.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/yacc.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/lex.prf \
		/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/include_source_dir.prf \
		/usr/local/Trolltech/Qt-4.6.2/lib/libQtGui.prl \
		/usr/local/Trolltech/Qt-4.6.2/lib/libQtCore.prl
	$(QMAKE) -unix VPATH=. -o qttmp-Debug.mk nbproject/qt-Debug.pro
/usr/local/Trolltech/Qt-4.6.2/mkspecs/common/g++.conf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/common/unix.conf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/common/linux.conf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/qconfig.pri:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/qt_functions.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/qt_config.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/exclusive_builds.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/default_pre.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/debug.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/default_post.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/warn_on.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/qt.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/unix/thread.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/moc.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/resources.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/uic.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/yacc.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/lex.prf:
/usr/local/Trolltech/Qt-4.6.2/mkspecs/features/include_source_dir.prf:
/usr/local/Trolltech/Qt-4.6.2/lib/libQtGui.prl:
/usr/local/Trolltech/Qt-4.6.2/lib/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -unix VPATH=. -o qttmp-Debug.mk nbproject/qt-Debug.pro

dist: 
	@$(CHK_DIR_EXISTS) nbproject/build/Debug/GNU-Linux-x86/echo1.0.0 || $(MKDIR) nbproject/build/Debug/GNU-Linux-x86/echo1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) nbproject/build/Debug/GNU-Linux-x86/echo1.0.0/ && $(COPY_FILE) --parents PCITableModel.h mainwindow.h nbproject/build/Debug/GNU-Linux-x86/echo1.0.0/ && $(COPY_FILE) --parents echo.qrc nbproject/build/Debug/GNU-Linux-x86/echo1.0.0/ && $(COPY_FILE) --parents mainwindow.cpp main.cpp PCITableModel.cpp nbproject/build/Debug/GNU-Linux-x86/echo1.0.0/ && $(COPY_FILE) --parents about.ui mainwindow.ui nbproject/build/Debug/GNU-Linux-x86/echo1.0.0/ && (cd `dirname nbproject/build/Debug/GNU-Linux-x86/echo1.0.0` && $(TAR) echo1.0.0.tar echo1.0.0 && $(COMPRESS) echo1.0.0.tar) && $(MOVE) `dirname nbproject/build/Debug/GNU-Linux-x86/echo1.0.0`/echo1.0.0.tar.gz . && $(DEL_FILE) -r nbproject/build/Debug/GNU-Linux-x86/echo1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) qttmp-Debug.mk


mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_mainwindow.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_mainwindow.cpp
moc_mainwindow.cpp: ui_mainwindow.h \
		PCITableModel.h \
		mainwindow.h
	/usr/local/Trolltech/Qt-4.6.2/bin/moc $(DEFINES) $(INCPATH) mainwindow.h -o moc_mainwindow.cpp

compiler_rcc_make_all: qrc_echo.cpp
compiler_rcc_clean:
	-$(DEL_FILE) qrc_echo.cpp
qrc_echo.cpp: echo.qrc
	/usr/local/Trolltech/Qt-4.6.2/bin/rcc -name echo echo.qrc -o qrc_echo.cpp

compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_about.h ui_mainwindow.h
compiler_uic_clean:
	-$(DEL_FILE) ui_about.h ui_mainwindow.h
ui_about.h: about.ui
	/usr/local/Trolltech/Qt-4.6.2/bin/uic about.ui -o ui_about.h

ui_mainwindow.h: mainwindow.ui
	/usr/local/Trolltech/Qt-4.6.2/bin/uic mainwindow.ui -o ui_mainwindow.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_rcc_clean compiler_uic_clean 

####### Compile

build/Debug/GNU-Linux-x86/mainwindow.o: mainwindow.cpp mainwindow.h \
		ui_mainwindow.h \
		PCITableModel.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/mainwindow.o mainwindow.cpp

build/Debug/GNU-Linux-x86/main.o: main.cpp mainwindow.h \
		ui_mainwindow.h \
		PCITableModel.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/main.o main.cpp

build/Debug/GNU-Linux-x86/PCITableModel.o: PCITableModel.cpp PCITableModel.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/PCITableModel.o PCITableModel.cpp

build/Debug/GNU-Linux-x86/moc_mainwindow.o: moc_mainwindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/moc_mainwindow.o moc_mainwindow.cpp

build/Debug/GNU-Linux-x86/qrc_echo.o: qrc_echo.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/qrc_echo.o qrc_echo.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

