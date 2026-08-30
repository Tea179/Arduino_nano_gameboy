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

enum gameState {pongMenu, pong_ONE_PLAYER, pong_TWO_PLAYER, pong_GAMEOVER};
gameState pongState = pongMenu;

void pongInit() {
  display.setTextColor(SSD1306_WHITE);
}

void pongUpdate() {
  bool pressed_left_u(digitalRead(BUTTON_LEFT_U) == LOW);
  bool pressed_right_u(digitalRead(BUTTON_RIGHT_U) == LOW);
  bool pressed_left_d(digitalRead(BUTTON_LEFT_D) == LOW);
  bool pressed_right_d(digitalRead(BUTTON_RIGHT_D) == LOW);

// Stany gry
  if (pongState == pongMenu) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(10,20);
    display.print("1. JEDNOOSOBOWY");
    display.setCursor(10,40);
    display.print("2. DWUOSOBOWY");

    display.display();
    
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
          state = GAMEOVER;
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