#include "display.h"
#include "input.h"
#include "pong.h"
#include "snake.h"
#include "tetris.h"
#include "flappy_bird.h"

enum GameState {MENU, PONG, SNAKE, TETRIS, FLAPPY_BIRD};
GameState state = MENU;

int selection = 0; // 0 = PONG; 1 = SNAKE; 2 = TETRIS; 3 = FlappyBird
bool lastButtonDown = false;
bool lastButtonUp = false;
bool lastButtonSelect = false;
const char* menuItems[] = {"PONG", "SNAKE", "TETRIS", "FLAPPY_BIRD"};

void drawMenu() {
    clearDisplay();
    for (int i = 0; i < 4; i++) {
        display.setCursor(0,i*15);
        display.print(menuItems[i]);
        if (i == selection) display.print("<--");
        delay(10);
    };
    renderFrame();
}

void setup() {
    initDisplay();
    initInput();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
}

void loop() {
    switch (state) {
        case MENU: {
            bool down = buttonPressed(0);
            bool up = buttonPressed(2);
            bool select = buttonPressed(1) || buttonPressed(3);

            if (down && !lastButtonDown) {
                selection = (selection + 1) % 4;
                drawMenu();
            }
            if (up && !lastButtonUp) {
                selection = (selection +3) % 4;
                drawMenu();
            }
            if (select && !lastButtonSelect) {
                state = (GameState)(selection +1);
            }

            lastButtonDown = down;
            lastButtonUp = up;
            lastButtonSelect = select;

            break;
        }
        case PONG: {
            pongUpdate();
            break;
        }
        case SNAKE: {
            snakeUpdate();
            break;
        }
        case TETRIS: {
            tetrisUpdate();
            break;
        }
        case FLAPPY_BIRD: {
            flappyBirdUpdate();
            break;
        }
    }
}