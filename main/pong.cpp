#include "pong.h"
#include "input.h"
#include "display.h"
#include <Arduino.h>

int px1 = 60;
int px2 = 60;
int bx = 60;
int by = 50;
int dx = 2;
int dy = 2;
const int ballW = 16;
const int ballH = 16;

unsigned long lastBallUpdate = 0;
unsigned long lastPlatformUpdate = 0;
const unsigned long ballInterval = 40;
const unsigned long platformInterval = 1;

enum pongGameState {pongMenu, pong_ONE_PLAYER, pong_TWO_PLAYER, MENU, pong_GAMEOVER};
pongGameState pongState = pongMenu;

int pongSelection = 0; // 0 = PONG; 1 = SNAKE; 2 = TETRIS; 3 = FlappyBird
bool pongLastButtonDown = false;
bool pongLastButtonUp = false;
bool pongLastButtonSelect = false;
const char* pongMenuItems[] = {"JEDNOOSOBOWY", "DWUOSOBOWY", "MAIN_MENU"};

bool exitToMainMenu = false;
bool pongExit() {
  bool result = exitToMainMenu;
  exitToMainMenu = false;
  return result;
}

void drawPongMenu() {
    clearDisplay();
    for (int i = 0; i < 3; i++) {
        display.setCursor(0,i*15);
        display.print(pongMenuItems[i]);
        if (i == pongSelection) display.print("<--");
        delay(10);
    };
    renderFrame();
}

void pongInit() {
  display.setTextColor(SSD1306_WHITE);
}

void pongUpdate() {
  bool pressed_left_u = buttonPressed(3);
  bool pressed_right_u = buttonPressed(2);
  bool pressed_left_d = buttonPressed(0);
  bool pressed_right_d = buttonPressed(1);

// Stany gry
  if (pongState == pongMenu) {
    
    bool down = buttonPressed(0);
    bool up = buttonPressed(2);
    bool select = buttonPressed(1) || buttonPressed(3);

    if (down && !pongLastButtonDown) {
        pongSelection = (pongSelection + 1) % 3;
        drawPongMenu();
    }
    if (up && !pongLastButtonUp) {
        pongSelection = (pongSelection +3) % 3;
        drawPongMenu();
    }
    if (select && !pongLastButtonSelect) {
        pongState = (pongGameState)(pongSelection +1);
    }

    pongLastButtonDown = down;
    pongLastButtonUp = up;
    pongLastButtonSelect = select;

    return;
    
    if (pressed_left_u || pressed_right_u) {
      bx = 60; by = 50;
      dx = 3; dy = 3;
      px1 = 60;
      pongState = pong_ONE_PLAYER;
      delay(200);
    }
    if (pressed_left_d || pressed_right_d) {
      bx = 60; by = 50;
      dx = 2; dy = 2;
      px1 = 60;
      px2 = 60;
      pongState = pong_TWO_PLAYER;
      delay(200);
    }
    return;
  }
  if (pongState == pong_GAMEOVER) {
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
      pongState = pongMenu;
      delay(200);
    }
    return;
  }
  if (pongState == MENU) {
    pongState = pongMenu;
    exitToMainMenu = true;
    return;
  }

// Tryb jednoosobowy
  if (pongState == pong_ONE_PLAYER) {
    display.setTextSize(2);
    // Przesuwanie się podstawy
    unsigned long nowPlatform = millis();
    if (nowPlatform - lastPlatformUpdate >= platformInterval) {
      lastPlatformUpdate = nowPlatform;
      if (pressed_left_d || pressed_left_u) {
        px1 -= 2;
      }
      if (pressed_right_d || pressed_right_u) {
        px1 += 2;
      }
      if (px1 <= 0) {
        px1 = 0;
      }
      if (px1 >= 100) {
        px1 = 100;
      }
    }

    // Mechanizm odbijania piłki
    unsigned long nowBall = millis();
    if (nowBall - lastBallUpdate >= ballInterval) {
      lastBallUpdate = nowBall;

      bx += dx;
      by += dy;

      if (bx <= 0) {
        bx = 0;
        dx = -dx;
      } else if (bx >= SCREEN_WIDTH - ballW) {
        bx = SCREEN_WIDTH - ballW;
        dx = -dx;
      }
      if (by <= 0) {
        by = 0;
        dy = -dy;
      } else if (by >= 60 - ballH) {
        if (bx + ballW >= px1 && bx <= px1 + 20) {
          by = 60 - ballH;
          dy = -dy;
        } else if (by >= SCREEN_HEIGHT - ballH) {
          bx = 60; by = 50;
          dx = 2; dy = 2;
          pongState = pong_GAMEOVER;
        }
      }
    }
  display.clearDisplay();
  display.setCursor(px1,50);
  display.print("__");
  display.setCursor(bx,by);
  display.print("o");
  display.display();
  }
// Tryb dwuosobowy
  if (pongState == pong_TWO_PLAYER) {
    display.setTextSize(2);
    // Przesuwanie się podstaw
    unsigned long nowPlatform = millis();
    if (nowPlatform - lastPlatformUpdate >= platformInterval) {
      lastPlatformUpdate = nowPlatform;
      // Dolna platforma
      if (pressed_left_d) {
        px1 -= 2;
      }
      if (pressed_right_d) {
        px1 += 2;
      }
      if (px1 <= 0) {
        px1 = 0;
      }
      if (px1 >= 100) {
        px1 = 100;
      }
      // Górna platforma
      if (pressed_left_u) {
        px2 -= 2;
      }
      if (pressed_right_u) {
        px2 += 2;
      }
      if (px2 <= 0 ) {
        px2 = 0;
      }
      if (px2 >= 100) {
        px2 = 100;
      }
    }

    // Mechanizm odbijania piłki
    unsigned long nowBall = millis();
    if (nowBall - lastBallUpdate >= ballInterval) {
      lastBallUpdate = nowBall;

      bx += dx;
      by += dy;

      if (bx <= 0) {
        bx = 0;
        dx = -dx;
      } else if (bx >= SCREEN_WIDTH - ballW) {
        bx = SCREEN_WIDTH - ballW;
        dx = -dx;
      }
      if (by <= 0) {
        if (bx + ballW >= px2 && bx <= px2 +20) {
          by = 0;
          dy = -dy;
        } else if (by <= SCREEN_HEIGHT + ballH) {
          bx = 60; by = 50;
          dx = 2; dy = 2;
          pongState = pong_GAMEOVER;
        }
      } else if (by >= 60 - ballH) {
        if (bx + ballW >= px1 && bx <= px1 + 20) {
          by = 60 - ballH;
          dy = -dy;
        } else if (by >= SCREEN_HEIGHT - ballH) {
          bx = 60; by = 50;
          dx = 2; dy = 2;
          pongState = pong_GAMEOVER;
        }
      }
    }
    display.clearDisplay();
    display.setCursor(px1,50);
    display.print("__");
    display.setCursor(px2,-10);
    display.print("__");
    display.setCursor(bx,by);
    display.print("o");
    display.display();
  }
}