#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;

void initDisplay();
void clearDisplay();
void renderFrame();

#endif