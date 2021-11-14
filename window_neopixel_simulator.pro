TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

TARGET = "window_neopixel_simulator"

SOURCES += \
        cometrain.cpp \
        main.cpp \
        plasma.cpp
LIBS += -lSDL2 -lSDL2main -lSDL2_gfx

HEADERS += \
	cometrain.h \
	plasma.h
