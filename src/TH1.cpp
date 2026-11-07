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