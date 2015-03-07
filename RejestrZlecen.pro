#-------------------------------------------------
#
# Project created by QtCreator 2014-12-18T21:53:49
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += printsupport
QT       += webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RejestrZlecen
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    konfiguracjabazy.cpp \
    konfig_uzytkownika.cpp \
    konfiguracja_kosztow.cpp \
    konfig_czynnosci.cpp \
    dod_zlecenia.cpp \
    kalendarz.cpp \
    raporty.cpp

HEADERS  += mainwindow.h \
    konfiguracjabazy.h \
    konfig_uzytkownika.h \
    konfiguracja_kosztow.h \
    konfig_czynnosci.h \
    dod_zlecenia.h \
    kalendarz.h \
    raporty.h

FORMS    += mainwindow.ui \
    konfiguracjabazy.ui \
    konfig_uzytkownika.ui \
    konfiguracja_kosztow.ui \
    konfig_czynnosci.ui \
    dod_zlecenia.ui \
    kalendarz.ui \
    raporty.ui

RESOURCES +=
