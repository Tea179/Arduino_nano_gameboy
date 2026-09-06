#include "buzzer.h"
#include <Arduino.h>

#define BUZZER_PIN 9

void initBuzzer() {
  pinMode(BUZZER_PIN, OUTPUT);
}

void buzz(unsigned int frequency, unsigned int durationMs) {
  tone(BUZZER_PIN, frequency, durationMs);
}

void buzzMenuMove() {
  buzz(1000,30);
}

void buzzSelect() {
  buzz(1500, 60);
}

void buzzGameOver() {
  buzz(500, 100);
}

void buzzTouch() {
  buzz(2000, 50);
}