#include <Arduino.h>
//B1 NON-BLOCKING MILLIS()
#include <Arduino.h>

#define LED1 23     
#define ADC_PIN 15    
unsigned long lanTruocLed = 0;
unsigned long lanTruocAdc = 0;
unsigned long lanTruocHeart = 0;

bool ledState = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(ADC_PIN, INPUT);
}

void loop() {
  unsigned long now = millis();

  // 1. Đổi trạng thái LED mỗi 500ms
  if (now - lanTruocLed >= 500) {
    lanTruocLed = now;
    ledState = !ledState;
    digitalWrite(LED1, ledState);
  }

  // 2. Đọc biến trở và in ra Serial Monitor mỗi 2000ms
  if (now - lanTruocAdc >= 2000) {
    lanTruocAdc = now;
    int tho = analogRead(ADC_PIN);
    Serial.println(tho);
  }

  // 3. In thông điệp heartbeat mỗi 5000ms
  if (now - lanTruocHeart >= 5000) {
    lanTruocHeart = now;
    Serial.println("heartbeat");
  }
}
