#include <Arduino.h>
//B3 FILTER
#define LED_PIN 23
#define POT_PIN 34
#define USE_FILTER 1

#define N 8
int buf[N] = {0};
int idx = 0;
long sum = 0;

int loc(int m) {
  sum -= buf[idx];
  buf[idx] = m;
  sum += m;
  idx = (idx + 1) % N;
  return sum / N;
}

void setup() {
  Serial.begin(115200);
  ledcSetup(0, 5000, 8);        // kênh 0, 5 kHz, 8 bit
  ledcAttachPin(LED_PIN, 0);    // gắn LED vào kênh 0
  analogReadResolution(12);     // 0-4095
}

void loop() {
  int tho = analogRead(POT_PIN);
  int l = loc(tho);

#if USE_FILTER
  int giaTri = l;
#else
  int giaTri = tho;
#endif

  ledcWrite(0, map(giaTri, 0, 4095, 0, 255));   // ghi theo kênh 0

  Serial.print(">tho:"); Serial.println(tho);
  Serial.print(">loc:"); Serial.println(l);
  delay(20);
}