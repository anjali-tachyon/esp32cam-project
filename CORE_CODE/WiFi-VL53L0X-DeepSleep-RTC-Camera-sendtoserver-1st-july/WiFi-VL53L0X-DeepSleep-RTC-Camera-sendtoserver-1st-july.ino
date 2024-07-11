#include "tach_wifi.h"

/*.....................................setup function..................................................*/
void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Disable brownout detector
  Serial.begin(115200);
  delay(1000);
  
   ++totalSleepTime;
   Serial.println("Total Sleep Time: " + String(totalSleepTime));
   pinMode(FLASH_LED_PIN, OUTPUT);

  // Initialize the function
  connectToWiFi();
  initSensor();
  initCamera();
  delay(2000); //Ensure enough delay for WiFi stack to stabilize
  initializeRTC();
    // Print current time, date, and day of the week after waking up
  printCurrentTime();
  printCurrentDate();
  printDayOfTheWeek();
  print_wakeup_reason();
  // Configure the wake-up source and sleep duration
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);  // 20 seconds
}

/*..........................................loop function ........................................................*/
void loop() {
  measureVL53L0X();
  delay(1000);  // Delay before next measurement
  //http://192.168.29.176:5000/list_images
  //http://192.168.29.176:5000/uploads/ESP32CAMCap_20240617_115956_a40fcbc0a5ee46fda5ae25b70b4a5788.jpg

}
