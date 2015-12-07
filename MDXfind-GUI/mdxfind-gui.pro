/*
 * Created 2014 by Sein Coray
 *
 * https://github.com/s3inlc/hashes-org-mgmt/tree/master/MDXfind-GUI
 */

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mdxfind-gui
TEMPLATE = app


SOURCES += main.cpp\
        start.cpp \
    options.cpp

HEADERS  += start.h \
    options.h

FORMS    += start.ui

RESOURCES +=
