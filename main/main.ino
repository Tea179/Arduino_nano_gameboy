#include "display.h"
#include "input.h"
#include "pong.h"
#include "snake.h"
#include "tetris.h"
#include "flappy_bird.h"

enum GameState {MENU, PONG, SNAKE, TETRIS, FLAPPY_BIRD};
GameState state = MENU;

void setup() {
    initDisplay();
    initInput();
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
    updateGame();
    renderFrame();
}