CONFIG += console c++14

SOURCES += \
    chess_GUI.cpp \
    main.cpp

HEADERS += \
    chess_GUI.h

macx {
    INCLUDEPATH += macos/include
    LIBS += $$PWD/macos/lib/libfltk.1.3.dylib
    LIBS += $$PWD/macos/lib/libfltk_images.1.3.dylib
    LIBS += $$PWD/macos/lib/libfltk_gl.1.3.dylib
    LIBS += $$PWD/macos/lib/libfltk_forms.1.3.dylib
}

win32 {
    INCLUDEPATH += win32/include
    LIBS += $$PWD/win32/lib/libfltk.a
    LIBS += $$PWD/win32/lib/libfltk_z.a
    LIBS += $$PWD/win32/lib/libfltk_images.a
    LIBS += $$PWD/win32/lib/libfltk_gl.a
    LIBS += $$PWD/win32/lib/libfltk_forms.a
}

