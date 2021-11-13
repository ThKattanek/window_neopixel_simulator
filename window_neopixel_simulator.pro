TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

TARGET = "window_neopixel_simulator"

SOURCES += \
        main.cpp
LIBS += -lSDL2 -lSDL2main -lSDL2_gfx
