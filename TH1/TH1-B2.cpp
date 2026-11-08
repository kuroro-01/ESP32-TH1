#include <Arduino.h>
// B2 DEBOUNCE
const int LED_PIN = 23;
const int BTN_PIN = 4;
const int CH = 0;

volatile uint32_t nhan = 0, tLan = 0;
volatile bool running = true;

void IRAM_ATTR onNut() {
  uint32_t t = millis();          // gọi được trong ISR
  if (t - tLan > 50) {            // bỏ qua sườn trong 50 ms
    nhan++;
    running = !running;
    tLan = t;
  }
}

int duty = 0, step = 1;
uint32_t lastStep = 0;
uint32_t nhanCu = 0;

void setup() {
  Serial.begin(115200);
  ledcSetup(CH, 5000, 8);
  ledcAttachPin(LED_PIN, CH);
  pinMode(BTN_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), onNut, FALLING);
}

void loop() {
  // fading không chặn
  if (running && millis() - lastStep >= 5) {
    lastStep = millis();
    duty += step;
    if (duty >= 255) { duty = 255; step = -1; }
    if (duty <= 0)   { duty = 0;   step = 1; }
    ledcWrite(CH, duty);
  }

  // in số lần nhấn khi có thay đổi (đọc nhan ngoài ISR)
  if (nhan != nhanCu) {
    nhanCu = nhan;
    Serial.printf("So lan nhan: %lu | %s\n", (unsigned long)nhanCu,
                  running ? "CHAY" : "DUNG");
  }
}
