TARGET = QuickWindow

DESTDIR = $$_PRO_FILE_PWD_/bin

TEMPLATE = app

contains(QT_MAJOR_VERSION, 4) {
    QT += declarative svg

    win32:QT += opengl
} else {
    QT += quick svg
}

contains(QT_MAJOR_VERSION, 5): win32:QT += winextras

contains(QT_MAJOR_VERSION, 4) {
    DEFINES += QT_4
} else:contains(QT_MAJOR_VERSION, 5) {
    DEFINES += QT_5
} else {
    DEFINES += QT_6
}

lessThan(QT_MAJOR_VERSION, 6) {
    DEFINES += QT_OLD
}

greaterThan(QT_MAJOR_VERSION, 4) {
    DEFINES += QT_NEW
}

win32: DEFINES += Q_WIN_BORDERLESS Q_OPENGL

unix:QMAKE_LFLAGS += "-Wl,-rpath,'\$$ORIGIN'"

include(src/global/global.pri)
include(src/controllers/controllers.pri)
include(src/kernel/kernel.pri)
include(src/declarative/declarative.pri)

INCLUDEPATH += src/global \
               src/controllers \
               src/kernel \
               src/declarative

OTHER_FILES += build.sh \
               README.md \
               LICENSE.md \
               content/qt4/Window.qml \
               content/qt5/Window.qml \
               .azure-pipelines.yml \
               .appveyor.yml \
