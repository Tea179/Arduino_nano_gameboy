// LEFT PIN - 3, RIGHT PIN - 1, TOP PIN - 2, BOTTOM PIN - 0

#include "input.h"
#include <Arduino.h>

const int pins[4] = {5,6,7,8};

void initInput() {
    for (int i = 0; i < 4; i++) {
        pinMode(pins[i], INPUT_PULLUP);
    }
}

bool buttonPressed(int index) {
    return digitalRead(pins[index]) == LOW;
}