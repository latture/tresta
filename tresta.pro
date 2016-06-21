#-------------------------------------------------
#
# Project created by QtCreator 2012-12-26T06:24:06
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -std=c++11 -fopenmp

macx{
    QMAKE_CXXFLAGS += -stdlib=libc++ -mmacosx-version-min=10.10
    LIBS += -stdlib=libc++ -mmacosx-version-min=10.10
}

CONFIG  += c++11 fopenmp
QT      += core gui opengl concurrent
TARGET   = tresta
TEMPLATE = app

INCLUDEPATH += $$PWD \
               $$PWD/ext \
               $$PWD/ext/eigen-3.2.5 \
               $$PWD/ext/boost_1_59_0 \
               $$PWD/ext/rapidjson/include \
               $$PWD/include

SOURCES += src/color_dialog.cpp \
           src/cylinder.cpp \
           src/demo_dialog.cpp \
           src/main.cpp \
           src/mainwindow.cpp \
           src/ply_exporter.cpp \
           src/setup.cpp \
           src/shape.cpp \
           src/sphere.cpp \
           src/truss_scene.cpp \
           src/window.cpp \
           ext/boost_1_59_0/libs/smart_ptr/src/sp_collector.cpp \
           ext/boost_1_59_0/libs/smart_ptr/src/sp_debug_hooks.cpp

HEADERS += include/abstract_scene.h \
           include/color_dialog.h \
           include/containers.h \
           include/csv_parser.h \
           include/cylinder.h \
           include/demo_dialog.h \
           include/glassert.h \
           include/mainwindow.h \
           include/ply_exporter.h \
           include/setup.h \
           include/shape.h \
           include/sphere.h \
           include/truss_scene.h \
           include/window.h

OTHER_FILES += assets/shaders/*
RESOURCES   += resources.qrc
FORMS       += src/mainwindow.ui
ICON         = assets/logo_64x64.png
DISTFILES   += assets/shaders/*
