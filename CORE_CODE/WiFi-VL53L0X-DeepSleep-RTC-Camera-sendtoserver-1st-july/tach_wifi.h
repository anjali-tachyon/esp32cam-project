#ifndef TACH_WIFI_H
#define TACH_WIFI_H

#include <WiFi.h>
#include "Adafruit_VL53L0X.h"
#include <Arduino.h> 
#include <Wire.h>
#include <esp_sleep.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"
#include <RTClib.h> // RTC

#define SDA 14
#define SCL 15
#define I2C_ADDRESS 0x29

// Macro for deep sleep
#define uS_TO_S_FACTOR 1000000 /* Conversion factor for microseconds to seconds */
#define TIME_TO_SLEEP 20       // Time ESP32 will go to sleep (in seconds)
#define ONE_MINUTE_MICROSECONDS (60 * uS_TO_S_FACTOR)
#define FIVE_SECONDS_MICROSECONDS (5 * uS_TO_S_FACTOR)

// Macro for ESP32CAM 
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22
#define FLASH_LED_PIN      4

// Global variables
//extern RTC_DATA_ATTR int totalSleepTime;
extern unsigned long totalSleepTime;
extern RTC_DS3231 rtc;
extern char daysOfTheWeek[7][12];

// Function declarations
void connectToWiFi();
void print_wakeup_reason();
void initSensor();
void initCamera();
void sendPhotoToServer();
void measureVL53L0X();
void initializeRTC();
void printCurrentTime();
void printCurrentDate();
void printDayOfTheWeek();

#endif
