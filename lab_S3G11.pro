QT += core gui widgets
CONFIG += c++17
TEMPLATE = app
TARGET = lab_S3G11

SOURCES += main.cpp mainwindow.cpp calculations.cpp
HEADERS += mainwindow.h calculations.h
FORMS += mainwindow.ui

win32-g++:QMAKE_CXXFLAGS += -finput-charset=UTF-8 -fexec-charset=UTF-8
win32-msvc:QMAKE_CXXFLAGS += /utf-8
