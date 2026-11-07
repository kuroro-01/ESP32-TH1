#include <Arduino.h>
#include <esp_sleep.h> // Thư viện chính thức cho Deep Sleep trên ESP32

#define LED_PIN 23
#define ADC_PIN 15

// GIỮ lại qua deep sleep (vùng nhớ RTC RAM)
RTC_DATA_ATTR int soLanThuc = 0; 

// MẤT qua deep sleep (RAM thông thường)
int demThuong = 0; 

void lamDeepSleep() {
  soLanThuc++;   
  
  demThuong++;   
  
  digitalWrite(LED_PIN, LOW); // Tắt LED trước khi ngủ
  
  Serial.println("Dang vao che do Deep Sleep trong 10 giay...");
  Serial.flush(); // Đợi truyền hết dữ liệu Serial trước khi ngủ

  // Cấu hình thức dậy bằng Timer sau 10 giây (10 giây = 10 * 1000000 microsecond)
  esp_sleep_enable_timer_wakeup(10ULL * 1000000ULL);
  esp_deep_sleep_start();
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(ADC_PIN, INPUT);

  // Nháy LED báo hiệu mạch đã thức dậy
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);

  Serial.println("\n--- DA THUC DAY ---");
  Serial.print("So lan thuc (RTC - GIU): ");
  Serial.println(soLanThuc);
  
  Serial.print("Gia tri demThuong (RAM thuong - MAT): ");
  Serial.println(demThuong); // Sẽ luôn về 0 sau mỗi lần thức dậy

  // Sau khi hiển thị thông tin, gọi hàm chuẩn bị ngủ tiếp sau 5 giây
  delay(5000);
  lamDeepSleep();
}

void loop() {
  // Không dùng gì trong loop vì ESP32 sẽ đi ngủ ở setup()
}