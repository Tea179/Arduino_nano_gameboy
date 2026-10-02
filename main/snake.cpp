#include "snake.h"
#include "display.h"
#include "input.h"
#include "buzzer.h"
#include <Arduino.h>

// MENU VARIABLES
enum snakeGameState {snakeMenu, snakeGame, MENU, snakeGameover};
snakeGameState snakeState = snakeMenu;

int snakeSelection = 1;
bool snakeLastButtonDown = false;
bool snakeLastButtonUp = false;
bool snakeLastButtonSelect = false;
const char* snakeMenuItems[] = {"Graj", "MENU"};

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

// GAME VARIABLES
  // grid
const int CELL = 4;
const int BOARD_SIZE = 60;
const int GRID_W = BOARD_SIZE / CELL;
const int GRID_H = BOARD_SIZE / CELL;
const int OFFSET_X = (128 - BOARD_SIZE) / 2;
const int OFFSET_Y = (64 - BOARD_SIZE) / 2;
  // snake
const int MAX_LENGTH = 64;
int snakeBodyX[MAX_LENGTH];
int snakeBodyY[MAX_LENGTH];
int snakeLength;
int newX;
int newY;
int snakeDirX;
int snakeDirY;
  // delay
unsigned long snakeLastMove = 0;
const unsigned long SNAKE_MOVE_INTERVAL = 100;
  // food
int foodX, foodY;

void spawnFood() {
  bool collision;
  do {
    collision = false;
    foodX = random(0, GRID_W);
    foodY = random(0, GRID_H);
    for (int i = 0; i < snakeLength; i++) {
      if (snakeBodyX[i] == foodX && snakeBodyY[i] == foodY) {
        collision = true;
        break;
      }
    } 
  } while (collision);
}

//scoreboard
int snake_score;
int snake_highscore = 0;

void snakeDrawScoreHUD() {
  display.setTextSize(1);
  display.setCursor(2,25);
  display.print("Wynik");
  display.setCursor(2,35);
  display.print(snake_score);
}

void snakeDrawGameOverScore() {
  display.setTextSize(2);
  display.setCursor(5,5);
  display.print("Wynik: ");
  display.print(snake_score);
  display.setTextSize(1);
  display.setCursor(5,20);
  display.print("Najlepszy wynik: ");
  display.print(snake_highscore);
}

void snakeInit() {
  display.setTextColor(SSD1306_WHITE);
  snakeLength = 3;
  for (int i = 0; i < snakeLength; i++) {
    snakeBodyX[i] = GRID_W/2 - i;
    snakeBodyY[i] = GRID_H/2;
  }
  snakeDirX = 1;
  snakeDirY = 0;
  spawnFood();

  snake_score = 0;
}
void snakeUpdate() {
  bool pressed_left_u = buttonPressed(3);
  bool pressed_right_u = buttonPressed(2);
  bool pressed_left_d = buttonPressed(0);
  bool pressed_right_d = buttonPressed(1);

  randomSeed(analogRead(A1));

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
      if (snakeState == snakeGame) {
        snakeInit();
      }
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
    if (millis() - snakeLastMove >= SNAKE_MOVE_INTERVAL) {
      snakeLastMove = millis();

      // moving

      if (buttonPressed(2) && snakeDirY !=1) {
        snakeDirX = 0;
        snakeDirY = -1;
      }
      if (buttonPressed(0) && snakeDirY != -1) {
        snakeDirX = 0;
        snakeDirY = 1;
      }
      if (buttonPressed(3) && snakeDirX != 1) {
        snakeDirX = -1;
        snakeDirY = 0;
      }
      if (buttonPressed(1) && snakeDirX != -1) {
        snakeDirX = 1;
        snakeDirY = 0;
      }

      newX = snakeBodyX[0] + snakeDirX;
      newY = snakeBodyY[0] + snakeDirY; 
    
    //colision

    bool hitWall = (newX < 0 || newX >= GRID_W || newY < 0 || newY >=GRID_H);

    bool hitSelf = false;
    for (int i = 0; i < snakeLength; i++){
      if (snakeBodyX[i] == newX && snakeBodyY[i] == newY) {
        hitSelf = true;
        break;
      }
    }
    
   if (hitWall || hitSelf) {
     buzzGameOver();
     if (snake_score > snake_highscore) snake_highscore = snake_score;
     snakeState = snakeGameover;
     return;
   }

    for (int i = snakeLength-1; i > 0; i--) {
      snakeBodyX[i] = snakeBodyX[i-1];
      snakeBodyY[i] = snakeBodyY[i-1];
    }

    snakeBodyX[0] = newX;
    snakeBodyY[0] = newY;
    
  // food
    bool ateFood = (newX == foodX && newY == foodY);

    if (ateFood && snakeLength < MAX_LENGTH) {
        snakeLength++;
        snake_score += 1;
        spawnFood();
        buzzTouch();
        renderFrame();
      }
    }

  // display  
  display.clearDisplay();

  display.drawRect(OFFSET_X - 1, OFFSET_Y - 1, BOARD_SIZE + 2, BOARD_SIZE + 2, SSD1306_WHITE);

  for (int i = 0; i < snakeLength; i++) {
    display.fillRect(OFFSET_X + snakeBodyX[i]*CELL, OFFSET_Y + snakeBodyY[i]*CELL, CELL, CELL, SSD1306_WHITE);
  }
  display.fillRect(OFFSET_X + foodX*CELL, OFFSET_Y + foodY*CELL, CELL, CELL, SSD1306_WHITE);
  snakeDrawScoreHUD();
  display.display();
  }

  // GAMEOVER
  if (snakeState == snakeGameover) {
    display.clearDisplay();
    snake_scoreboard();
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