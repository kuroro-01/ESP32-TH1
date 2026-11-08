#include <Arduino.h>
// B1 DELAY()
#define LED1 23  
#define ADC_PIN 15

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(ADC_PIN, INPUT);
}

void loop() {
  // 1. Nháy LED (bật 500ms, tắt 500ms)
  digitalWrite(LED1, HIGH);
  delay(500);
  digitalWrite(LED1, LOW);
  delay(500);

  // 2. Đọc biến trở và in ra Serial Monitor
  int tho = analogRead(ADC_PIN);
  Serial.println(tho);
  delay(2000);

  // 3. In thông điệp heartbeat
  Serial.println("heartbeat");
  delay(5000);
}