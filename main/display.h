#ifndef DISPLAY_H
#define DISPLAY_H
#define SCREEN_HEIGHT 64
#define SCREEN_WIDTH 128

#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;

void initDisplay();
void clearDisplay();
void renderFrame();

#endif