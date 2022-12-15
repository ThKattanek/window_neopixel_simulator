TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

TARGET = "window_neopixel_simulator"

SOURCES += \
        SDL2_gfxPrimitives.c \
        SDL2_rotozoom.c \
        cometrain.cpp \
        fire.cpp \
        main.cpp \
        # pictureshow.cpp \
        pixeltest.cpp \
        plasma.cpp \
        effect_kordesii.cpp
LIBS += -lSDL2 -lSDL2main #-lpng

HEADERS += \
	cometrain.h \
	fire.h \
        # pictureshow.h \
	pixeltest.h \
        plasma.h \
        effect_kordesii.h
