#include "tetris.h"
#include "buzzer.h"
#include "input.h"
#include "display.h"
#include <Arduino.h>

// MENU VARIABLES
enum tetrisGameState {tetrisMenu, tetrisGame, MENU, tetrisGameover};
tetrisGameState tetrisState = tetrisMenu;

int tetrisSelection = 1;
bool tetrisLastButtonDown = false;
bool tetrisLastButtonUp = false;
bool tetrisLastButtonSelect = false;
const char* tetrisMenuItems[] = {"Graj", "MENU"};

bool tetrisExitToMainMenu = false;
bool tetrisExit() {
  bool result = tetrisExitToMainMenu;
  tetrisExitToMainMenu = false;
  return result;
}

bool tetrisMenuNeedsDraw = true;

void drawTetrisMenu() {
    display.setTextSize(1);
    clearDisplay();
    for (int i = 0; i < (int)(sizeof(tetrisMenuItems)/sizeof(tetrisMenuItems[0])); i++) {
        display.setCursor(0,i*15);
        display.print(tetrisMenuItems[i]);
        if (i == tetrisSelection) display.print("<--");
        delay(10);
    };
    renderFrame();
}

// delay
unsigned long tetrisLastMove = 0;
const unsigned long TETRIS_MOVE_INTERVAL = 400;

// 

void tetrisInit() {
  display.setTextColor(SSD1306_WHITE);
}

void tetrisUpdate() {
  // Stany gry
  if (tetrisState == tetrisMenu) {
    if (tetrisMenuNeedsDraw) {
      drawTetrisMenu();
      tetrisMenuNeedsDraw = false;
    }
    
    bool down = buttonPressed(0);
    bool up = buttonPressed(2);
    bool select = buttonPressed(1) || buttonPressed(3);

    if (down && !tetrisLastButtonDown) {
        tetrisSelection = (tetrisSelection + 1) % 2;
        buzzMenuMove();
        drawTetrisMenu();
    }
    if (up && !tetrisLastButtonUp) {
        tetrisSelection = (tetrisSelection +3) % 2;
        buzzMenuMove();
        drawTetrisMenu();
    }
    if (select && !tetrisLastButtonSelect) {
      tetrisState = (tetrisGameState)(tetrisSelection +1);
      if (tetrisState == tetrisGame) {
        tetrisInit();
      }
      buzzSelect();
      return;
    }

    tetrisLastButtonDown = down;
    tetrisLastButtonUp = up;
    tetrisLastButtonSelect = select;

    return;
  }
  // Return to main menu
  if (tetrisState == MENU) {
    tetrisState = tetrisMenu;
    tetrisExitToMainMenu = true;
    tetrisMenuNeedsDraw = true;
    return;
  }

  // GAME
  if (tetrisState == tetrisGame) {
    if (millis() - tetrisLastMove >= TETRIS_MOVE_INTERVAL) {
      tetrisLastMove = millis();
    }
  }
}
