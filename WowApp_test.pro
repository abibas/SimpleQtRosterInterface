QT       += core gui concurrent network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
PKGCONFIG += openssl
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    contact_info_dialog.cpp \
    contacts_tree_item.cpp \
    contacts_tree_model.cpp \
    custom_json_processor.cpp \
    downloader.cpp \
    icons_generator.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    contact_info.h \
    contact_info_dialog.h \
    contacts_tree_item.h \
    contacts_tree_model.h \
    custom_json_processor.h \
    downloader.h \
    icons_generator.h \
    mainwindow.h

FORMS += \
    contact_info_dialog.ui \
    mainwindow.ui
