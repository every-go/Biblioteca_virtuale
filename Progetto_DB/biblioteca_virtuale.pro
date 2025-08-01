######################################################################
# Automatically generated by qmake (3.1) Sat Jan 4 14:40:38 2025
######################################################################

TEMPLATE = app
TARGET = biblioteca_virtuale
INCLUDEPATH += .
QT += widgets core gui sql

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += interfaccia_grafica/adminarea.h \
           DB/dbobserver.h \
           DB/dbmanager.h \
           DB/dbdelete.h \
           DB/dbcreate.h \
           DB/dbupdate.h \
           interfaccia_grafica/librarymanager.h \
           interfaccia_grafica/visitor.h \
           interfaccia_grafica/visitorwidget.h \
           interfaccia_grafica/mainwindow.h \
           interfaccia_grafica/userarea.h \
           modello_logico/biblioteca.h \
           modello_logico/cartaceo.h \
           modello_logico/cd.h \
           modello_logico/film.h \
           modello_logico/libri.h \
           modello_logico/manga.h \
           modello_logico/multimedia.h \
           modello_logico/riviste.h
SOURCES += main.cpp \
           DB/dbmanager.cpp \
           DB/dbdelete.cpp \
           DB/dbcreate.cpp \
           DB/dbupdate.cpp \
           interfaccia_grafica/adminarea.cpp \
           interfaccia_grafica/librarymanager.cpp \
           interfaccia_grafica/visitorwidget.cpp \
           interfaccia_grafica/mainwindow.cpp \
           interfaccia_grafica/userarea.cpp \
           modello_logico/biblioteca.cpp \
           modello_logico/cartaceo.cpp \
           modello_logico/cd.cpp \
           modello_logico/film.cpp \
           modello_logico/libri.cpp \
           modello_logico/manga.cpp \
           modello_logico/multimedia.cpp \
           modello_logico/riviste.cpp
RESOURCES += IMG/resources.qrc
