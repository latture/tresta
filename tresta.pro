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

CONFIG += c++11 fopenmp

QT       += core gui opengl

TARGET = Tresta
TEMPLATE = app

INCLUDEPATH += $$PWD/ext \
               $$PWD/ext/eigen-3.2.5 \
               $$PWD/ext/boost_1_59_0 \
               $$PWD/include \
               $$PWD/ext/rapidjson/include \
               $$PWD

SOURCES += src/main.cpp \
           src/cylinder.cpp \
           src/setup.cpp \
           src/sphere.cpp \
           src/truss_scene.cpp \
           src/window.cpp \
           src/mainwindow.cpp \
           ext/boost_1_59_0/libs/smart_ptr/src/sp_collector.cpp \
           ext/boost_1_59_0/libs/smart_ptr/src/sp_debug_hooks.cpp

HEADERS += include/containers.h \
           include/cylinder.h \
           include/sphere.h \
           include/abstract_scene.h \
           include/truss_scene.h \
           include/window.h \
           include/csv_parser.h \
           include/glassert.h \
           include/setup.h \
           include/mainwindow.h

OTHER_FILES += assets/shaders/*

RESOURCES += resources.qrc

FORMS += src/mainwindow.ui

ICON = assets/logo_64x64.png

DISTFILES += src/shaders/*
