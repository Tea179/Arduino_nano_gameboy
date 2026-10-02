#include "flappy_bird.h"
#include "buzzer.h"
#include "input.h"
#include "display.h"
#include <Arduino.h>

// MENU VARIABLES
enum flappy_birdGameState {flappy_birdMenu, flappy_birdGame, MENU, flappy_birdGameover};
flappy_birdGameState flappy_birdState = flappy_birdMenu;

int flappy_birdSelection = 1;
bool flappy_birdLastButtonDown = false;
bool flappy_birdLastButtonUp = false;
bool flappy_birdLastButtonSelect = false;
const char* flappy_birdMenuItems[] = {"Graj", "MENU"};

bool flappy_birdExitToMainMenu = false;
bool flappyBirdExit() {
  bool result = flappy_birdExitToMainMenu;
  flappy_birdExitToMainMenu = false;
  return result;
}

bool flappy_birdMenuNeedsDraw = true;

void drawFlappyBirdMenu() {
    display.setTextSize(1);
    clearDisplay();
    for (int i = 0; i < (int)(sizeof(flappy_birdMenuItems)/sizeof(flappy_birdMenuItems[0])); i++) {
        display.setCursor(0,i*15);
        display.print(flappy_birdMenuItems[i]);
        if (i == flappy_birdSelection) display.print("<--");
        delay(10);
    };
    renderFrame();
}

// GAME VARIABLES
  // grid
const int CELL = 4;

// delay
unsigned long birdLastMove = 0;
unsigned long obstacleLastMove = 0;
const unsigned long BIRD_MOVE_INTERVAL = 100;
const unsigned long OBSTACLE_MOVE_INTERVAL = 400;

//scoreboard
int flappy_bird_score;
int flappy_bird_highscore = 0;

void flappyBirdDrawGameOverScore() {
  display.setTextSize(2);
  display.setCursor(5,5);
  display.print("Wynik: ");
  display.print(flappy_bird_score);
  display.setTextSize(1);
  display.setCursor(5,20);
  display.print("Najlepszy wynik: ");
  display.print(flappy_bird_highscore);
}

// bird moving
int birdX;
int birdY;

void flappyBirdInit() {
  display.setTextColor(SSD1306_WHITE);

  birdX = 10;
  birdY = 30;

  flappy_bird_score = 0;
}

void flappyBirdUpdate() {
  bool birdJump = buttonPressed(0) || buttonPressed(1) || buttonPressed(2) || buttonPressed(3);

  // Stany gry
  if (flappy_birdState == flappy_birdMenu) {
    if (flappy_birdMenuNeedsDraw) {
      drawFlappyBirdMenu();
      flappy_birdMenuNeedsDraw = false;
    }
    
    bool down = buttonPressed(0);
    bool up = buttonPressed(2);
    bool select = buttonPressed(1) || buttonPressed(3);

    if (down && !flappy_birdLastButtonDown) {
        flappy_birdSelection = (flappy_birdSelection + 1) % 2;
        buzzMenuMove();
        drawFlappyBirdMenu();
    }
    if (up && !flappy_birdLastButtonUp) {
        flappy_birdSelection = (flappy_birdSelection +3) % 2;
        buzzMenuMove();
        drawFlappyBirdMenu();
    }
    if (select && !flappy_birdLastButtonSelect) {
      flappy_birdState = (flappy_birdGameState)(flappy_birdSelection +1);
      if (flappy_birdState == flappy_birdGame) {
        flappyBirdInit();
      }
      buzzSelect();
      return;
    }

    flappy_birdLastButtonDown = down;
    flappy_birdLastButtonUp = up;
    flappy_birdLastButtonSelect = select;

    return;
  }
  // Return to main menu
  if (flappy_birdState == MENU) {
    flappy_birdState = flappy_birdMenu;
    flappy_birdExitToMainMenu = true;
    flappy_birdMenuNeedsDraw = true;
    return;
  }

  // GAME
  if (flappy_birdState == flappy_birdGame) {
    // bird
    if (millis() - birdLastMove >= BIRD_MOVE_INTERVAL) {
      birdLastMove = millis();

      // jumping
      birdX += 3;

      if (birdJump) {
        birdY += 5;
        delay(200);
        birdY -= 5;
      }
    }
  }

  // GAMEOVER
  if (flappy_birdState == flappy_birdGameover) {
    display.clearDisplay();
    flappyBirdDrawGameOverScore();
    display.setCursor(2,35);
    display.print("Nacisnij dowolny");
    display.setCursor(2,45);
    display.print("Przycisk");
    display.display();

    if (birdJump) {
      flappy_birdState = flappy_birdMenu;
      delay(200);
      flappy_birdMenuNeedsDraw = true;
    }
    return;
  }
}