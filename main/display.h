#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;

SCREEN_HEIGHT 64
SCREEN_WIDTH 128

void initDisplay();
void clearDisplay();
void renderFrame();

#endif