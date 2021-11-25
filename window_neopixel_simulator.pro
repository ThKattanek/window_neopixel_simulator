TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

TARGET = "window_neopixel_simulator"

SOURCES += \
        cometrain.cpp \
        fire.cpp \
        main.cpp \
        pictureshow.cpp \
        plasma.cpp
LIBS += -lSDL2 -lSDL2main -lSDL2_gfx -lpng

HEADERS += \
	cometrain.h \
	fire.h \
	pictureshow.h \
	plasma.h
