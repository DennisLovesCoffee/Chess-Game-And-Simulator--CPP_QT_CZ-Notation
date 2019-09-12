#-------------------------------------------------
#
# Project created by QtCreator 2019-02-26T11:43:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chess
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    game.cpp \
    main.cpp \
    chessboard.cpp \
    chessbox.cpp \
    button.cpp \
    chesspiece.cpp \
    king.cpp \
    queen.cpp \
    rook.cpp \
    pawn.cpp \
    knight.cpp \
    bishop.cpp \
    tabmanagment.cpp \
    filework.cpp \
    transcriptscrollarea.cpp

HEADERS += \
    game.h \
    chessboard.h \
    chessbox.h \
    button.h \
    chesspiece.h \
    king.h \
    queen.h \
    rook.h \
    pawn.h \
    knight.h \
    bishop.h \
    tabmanagment.h \
    filework.h \
    transcriptscrollarea.h

FORMS +=

RESOURCES += \
    resources.qrc
