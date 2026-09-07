#include "battery.h"
#include <Arduino.h>

#define BATTERY_PIN A0
#define LED_G_PIN 4
#define LED_Y_PIN 3
#define LED_R_PIN 2

#define DIVIDER_RATIO 2.0

#define BATTERY_MAX_V 6.4 // 2x CR2032
#define BATTERY_MIN_V 4.4 // 2x CR2032

const unsigned long BATTERY_CHECK_INTERVAL = 5000; //ms
unsigned long lastBatteryCheck = 0;

void initBattery(){
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_Y_PIN, OUTPUT);
  pinMode(LED_R_PIN, OUTPUT);
}

float readBatteryVoltage() {
  int raw = analogRead(BATTERY_PIN);
  float vAdc = (raw / 1023.0) * 5.0;
  return vAdc * DIVIDER_RATIO;
}

int batteryPercent() {
  float v = readBatteryVoltage();
  if (v >= BATTERY_MAX_V) return 100;
  if (v <= BATTERY_MIN_V) return 0;
  return (int)(((v - BATTERY_MIN_V) / (BATTERY_MAX_V - BATTERY_MIN_V)) * 100.0);
}

void setBatteryLED(int percent) {
  digitalWrite(LED_G_PIN, LOW);
  digitalWrite(LED_Y_PIN, LOW);
  digitalWrite(LED_R_PIN, LOW);

  if (percent > 50) {
    digitalWrite(LED_G_PIN, HIGH);
  } else if (percent > 20) {
    digitalWrite(LED_Y_PIN, HIGH);
  } else {
    digitalWrite(LED_R_PIN, HIGH);
  }
}

void updateBatteryLED() {
  unsigned long now = millis();
  if (now - lastBatteryCheck >= BATTERY_CHECK_INTERVAL) {
    lastBatteryCheck = now;
    setBatteryLED(batteryPercent());
  }
}