#include "snake.h"
#include "display.h"
#include "input.h"
#include "buzzer.h"
#include <Arduino.h>

enum snakeGameState {snakeMenu, snakeGame, MENU, snakeGameover};
snakeGameState snakeState = snakeMenu;

int snakeSelection = 1; // 0 = PONG; 1 = SNAKE; 2 = TETRIS; 3 = FlappyBird
bool snakeLastButtonDown = false;
bool snakeLastButtonUp = false;
bool snakeLastButtonSelect = false;
const char* snakeMenuItems[] = {"Graj", "MENU"};

int snakeHeadX = 60;
int snakeHeadY = 30;

bool snakeExitToMainMenu = false;
bool snakeExit() {
  bool result = snakeExitToMainMenu;
  snakeExitToMainMenu = false;
  return result;
}

bool snakeMenuNeedsDraw = true;

void drawSnakeMenu() {
    display.setTextSize(1);
    clearDisplay();
    for (int i = 0; i < (int)(sizeof(snakeMenuItems)/sizeof(snakeMenuItems[0])); i++) {
        display.setCursor(0,i*15);
        display.print(snakeMenuItems[i]);
        if (i == snakeSelection) display.print("<--");
        delay(10);
    };
    renderFrame();
}

void snakeInit() {
  display.setTextColor(SSD1306_WHITE);
}
void snakeUpdate() {
  bool pressed_left_u = buttonPressed(3);
  bool pressed_right_u = buttonPressed(2);
  bool pressed_left_d = buttonPressed(0);
  bool pressed_right_d = buttonPressed(1);

  // Stany gry
  if (snakeState == snakeMenu) {
    if (snakeMenuNeedsDraw) {
      drawSnakeMenu();
      snakeMenuNeedsDraw = false;
    }
    
    bool down = buttonPressed(0);
    bool up = buttonPressed(2);
    bool select = buttonPressed(1) || buttonPressed(3);

    if (down && !snakeLastButtonDown) {
        snakeSelection = (snakeSelection + 1) % 2;
        buzzMenuMove();
        drawSnakeMenu();
    }
    if (up && !snakeLastButtonUp) {
        snakeSelection = (snakeSelection +3) % 2;
        buzzMenuMove();
        drawSnakeMenu();
    }
    if (select && !snakeLastButtonSelect) {
      snakeState = (snakeGameState)(snakeSelection +1);
      buzzSelect();
      return;
    }

    snakeLastButtonDown = down;
    snakeLastButtonUp = up;
    snakeLastButtonSelect = select;

    return;
  }
  // Return to main menu
  if (snakeState == MENU) {
    snakeState = snakeMenu;
    snakeExitToMainMenu = true;
    snakeMenuNeedsDraw = true;
    return;
  }

  // GAME
  if (snakeState == snakeGame) {
    // wall colision
    if (snakeHeadX <= 0 || snakeHeadX >= 128) {
      snakeHeadX = 60;
      snakeHeadY = 30;
      buzzGameOver();
      snakeState = snakeGameover;
    } else if (snakeHeadY <= 0 || snakeHeadY >= 64) {
      snakeHeadX = 60;
      snakeHeadY = 30;
      buzzGameOver();
      snakeState = snakeGameover;
    }
    // moving
    while (snakeHeadX) {
      case (buttonPressed(1)) {
        snakeHeadX += 1;
        delay(50);
      }
      case (buttonPressed(3)) {
        snakeHeadX -+ 1;
        delay(50);
      }
    }
    while (snakeHeadY) {
      case (buttonPressed(0)) {
        snakeHeadY += 1;
        delay(50);
      }
      case (buttonPressed(2)) {
        snakeHeadY -= 1;
        delay(50);
      }
    }
  display.clearDisplay();
  display.setCursor(snakeHeadX, snakeHeadY);
  display.print("■");
  display.display();
  }

  // GAMEOVER
  if (snakeState == snakeGameover) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(5,15);
    display.print("Koniec Gry");
    display.setTextSize(1);
    display.setCursor(2,35);
    display.print("Nacisnij dowolny");
    display.setCursor(2,45);
    display.print("Przycisk");
    display.display();

    if (pressed_left_u || pressed_right_u || pressed_left_d || pressed_right_d) {
      snakeState = snakeMenu;
      delay(200);
      snakeMenuNeedsDraw = true;
    }
    return;
  }
}