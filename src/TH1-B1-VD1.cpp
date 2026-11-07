#include <Arduino.h>
//B1 LED FADING NO INTERUPT
const int LED_PIN = 23;
const int CH = 0;

void setup() {
  ledcSetup(CH, 5000, 8);      // kênh 0, 5 kHz, 8 bit (0-255)
  ledcAttachPin(LED_PIN, CH);
}

void loop() {
  for (int d = 0; d <= 255; d++) {
    ledcWrite(CH, d);
    delay(5);
  }
  for (int d = 255; d >= 0; d--) {
    ledcWrite(CH, d);
    delay(5);
  }
}