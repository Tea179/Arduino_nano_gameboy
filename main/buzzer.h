#ifndef BUZZER_H
#define BUZZER_H

void initBuzzer();
void buzz(unsigned int frequency, unsigned int durationMs);
void buzzMenuMove();
void buzzSelect();
void buzzGameOver();
void buzzTouch();

#endif