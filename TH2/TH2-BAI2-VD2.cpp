
//BAI 2 Thức dậy chạy lại từ setup() kết hợp Web Server
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#define LED_PIN 23
#define ADC_PIN 15

// Khởi tạo Web Server ở cổng 80
WebServer server(80);

// Thông tin mạng Wi-Fi phát ra từ ESP32
const char* ssid = "ESP32_DeepSleep_AP";
const char* password = "12345678";

// Biến đếm số lần thức dậy lưu bằng RTC
RTC_DATA_ATTR int soLanThucWeb = 0;

// Hàm xử lý khi có client truy cập trang chủ web
void handleRoot() {
  int adcValue = analogRead(ADC_PIN);
  
  String html = "<html><head><meta charset='UTF-8'><title>ESP32 Deep Sleep Demo</title></head><body>";
  html += "<h1>ESP32 Thuc Day Tu Setup</h1>";
  html += "<p><b>So lan thuc day:</b> " + String(soLanThucWeb) + "</p>";
  html += "<p><b>Gia tri bien tro (D15):</b> " + String(adcValue) + "</p>";
  html += "<p><a href='/sleep'><button style='padding:10px 20px; font-size:16px;'>Cho ESP32 Di Ngu Ngay</button></a></p>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

// Hàm chuyển thiết bị sang trạng thái ngủ sâu
void handleSleep() {
  server.send(200, "text/html", "<h3>ESP32 dang di ngu trong 10 giay... Ban se mat ket noi Wi-Fi.</h3>");
  delay(1000);
  
  digitalWrite(LED_PIN, LOW);
  esp_sleep_enable_timer_wakeup(10ULL * 1000000ULL); // Ngủ 10 giây
  esp_deep_sleep_start();
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(ADC_PIN, INPUT);

  soLanThucWeb++;

  // Kiểm tra nguyên nhân thức dậy[cite: 4]
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  Serial.print("Nguyen nhân thức dậy: ");
  if (wakeup_reason == ESP_SLEEP_WAKEUP_TIMER) {
    Serial.println("Thức dậy do Timer (Hết giờ ngủ)");
  } else {
    Serial.println("Khởi động lạnh (Reset / Cấp nguồn mới)");
  }

  // Khởi động Wi-Fi Access Point
  Serial.println("Dang khoi tao Wi-Fi AP...");
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Dia chi IP AP: ");
  Serial.println(IP);

  // Định nghĩa các đường dẫn (routes) cho Web Server
  server.on("/", handleRoot);
  server.on("/sleep", handleSleep);
  server.begin();
  Serial.println("HTTP server đã khởi động");
}

unsigned long lastLedToggle = 0;
bool ledState = false;

void loop() {
  // Lắng nghe các yêu cầu từ trình duyệt web[cite: 4]
  server.handleClient();

  // LED nhấp nháy không chặn (non-blocking) để báo hiệu CPU vẫn đang chạy[cite: 4]
  unsigned long currentMillis = millis();
  if (currentMillis - lastLedToggle >= 500) {
    lastLedToggle = currentMillis;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }
}