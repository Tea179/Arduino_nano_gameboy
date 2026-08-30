#include "display.h"
#include "input.h"
#include "pong.h"
#include "snake.h"
#include "tetris.h"
#include "flappy_bird.h"

enum GameState {MENU, PONG, SNAKE, TETRIS, FLAPPY_BIRD};
GameState state = MENU;

int yPong = 0;
int ySnake = 15;
int yTetris = 30;
int yFlappyBird = 45;
int y = yPong;

void setup() {
    initDisplay();
    initInput();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
}

void loop() {
    switch (state) {
        case MENU:
            break;
        case PONG:
            pongUpdate();
            break;
        case SNAKE:
            snakeUpdate();
            break;
        case TETRIS:
            tetrisUpdate();
            break;
        case FLAPPY_BIRD:
            flappyBirdUpdate();
            break;
    }
    // Selecting
    if (y = yPong) {
        clearDisplay();
        display.setCursor(10, yPong);
        display.print("PONG <--");
        display.setCursor(10, ySnake);
        display.print("SNAKE");
        display.setCursor(10, yTetris);
        display.print("TETRIS");
        display.setCursor(10, yFlappyBird);
        display.print("FLAPPY_BIRD");
        renderFrame();
    }
    else if (y == yPong && buttonPressed(5)) {
        y = ySnake;
        clearDisplay();
        display.setCursor(10, yPong);
        display.print("PONG");
        display.setCursor(10, ySnake);
        display.print("SNAKE <--");
        display.setCursor(10, yTetris);
        display.print("TETRIS");
        display.setCursor(10, yFlappyBird);
        display.print("FLAPPY_BIRD");
        renderFrame();
    }
    else if (y == ySnake && buttonPressed(5)) {
        y = yTetris;
        clearDisplay();
        display.setCursor(10, yPong);
        display.print("PONG");
        display.setCursor(10, ySnake);
        display.print("SNAKE");
        display.setCursor(10, yTetris);
        display.print("TETRIS <--");
        display.setCursor(10, yFlappyBird);
        display.print("FLAPPY_BIRD");
        renderFrame();
    }
    else if (y == yTetris && buttonPressed(5)) {
        y = yFlappyBird;
        clearDisplay();
        display.setCursor(10, yPong);
        display.print("PONG");
        display.setCursor(10, ySnake);
        display.print("SNAKE <--");
        display.setCursor(10, yTetris);
        display.print("TETRIS");
        display.setCursor(10, yFlappyBird);
        display.print("FLAPPY_BIRD <--");
        renderFrame();
    }
}