TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += sdl2
}
windows {
    INCLUDEPATH += C:/SDL2-2.0.3/include
    LIBS += -LC:/SDL2-2.0.3/lib/x64
    LIBS += -lSDL2
    LIBS += -lSDL2main
}
