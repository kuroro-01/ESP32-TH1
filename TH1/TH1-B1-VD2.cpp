#include <Arduino.h>
 //B1 LED FADING WITH INTERUPT
 const int LED_PIN = 23;
const int BTN_PIN = 4;
const int CH = 0;

volatile bool running = true;
volatile unsigned long lastPress = 0;

void IRAM_ATTR onButton() {
  unsigned long now = millis();
  if (now - lastPress > 200) {   // chống rung phím
    running = !running;
    lastPress = now;
  }
}

int duty = 0;
int step = 1;
unsigned long lastStep = 0;

void setup() {
  ledcSetup(CH, 5000, 8);
  ledcAttachPin(LED_PIN, CH);
  pinMode(BTN_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), onButton, FALLING);
}

void loop() {
  if (running && millis() - lastStep >= 5) {
    lastStep = millis();
    duty += step;
    if (duty >= 255) { duty = 255; step = -1; }
    if (duty <= 0)   { duty = 0;   step = 1; }
    ledcWrite(CH, duty);
  }
}
