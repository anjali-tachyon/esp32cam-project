#include "tach_wifi.h"

const char* ssid = "Tachyondyno_4G";
const char* password = "PYTH_a2+b2=c2";

/*...Variable declaration for HTTP server...*/
String serverName = "192.168.29.176";
String serverPath = "/upload";
const int serverPort = 5000;
bool LED_Flash_ON = true;
WiFiClient client;

// declaring variable for VL53L0X
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// variable Declaration of RTC 
//RTC_DATA_ATTR int totalSleepTime = 0;
unsigned long totalSleepTime = 0;

RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


/*.............Connected to wifi.................................*/
void connectToWiFi() {
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Free heap after WiFi connection: ");
  Serial.println(ESP.getFreeHeap());
}

/*...............reason for wake-up function..........*/
void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0: Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1: Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER: Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP: Serial.println("Wakeup caused by ULP program"); break;
    default: Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}

/*...............sensor setup function definition.................*/
void initSensor() {
  Wire.begin(SDA, SCL);
   Serial.println("I2C bus initialized.");
  if (!lox.begin(0x29)) {  
    
    Serial.println("Could not find a valid VL53L0X sensor, check wiring!");
    while (1);
  }
  Serial.println(F("VL53L0X initialized"));
}

/*..........................Sensor Measurement Function.........*/
void measureVL53L0X() {
    VL53L0X_RangingMeasurementData_t measure;

    Serial.print("Reading a measurement... ");

    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

    if (measure.RangeStatus != 4 && measure.RangeMilliMeter != 0) {  // phase failures have incorrect data
        int distance = measure.RangeMilliMeter;
        Serial.print("Distance (mm): ");
        Serial.println(distance);

        if (distance < 100) {
            Serial.println("Sleeping for 5 seconds");
            delay(5000); // Sleep for 5 seconds
            totalSleepTime += FIVE_SECONDS_MICROSECONDS;
        } else {
            sendPhotoToServer();
            Serial.println("Sleeping for 1 minute");

            // Calculate remaining time to sleep to ensure total sleep time is 1 minute
            unsigned long remainingSleepTimeMicroseconds = ONE_MINUTE_MICROSECONDS - totalSleepTime;
            esp_sleep_enable_timer_wakeup(remainingSleepTimeMicroseconds);

            esp_deep_sleep_start();
        }
    } else {
        Serial.print("Invalid Range Status: ");
        Serial.println(measure.RangeStatus);
        Serial.println("Retrying...");
        delay(500); // Small delay before retrying
    }

    Serial.println("Out of range");
}

void initCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  Serial.println("Camera initialize successfully");
  sensor_t * s = esp_camera_sensor_get();
  s->set_brightness(s, 2);     // -2 to 2
  s->set_contrast(s, 2);       // -2 to 2
  s->set_saturation(s, 2);     // -2 to 2
  s->set_special_effect(s, 0); // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
  s->set_whitebal(s, 1);       // 0 = disable , 1 = enable
  s->set_awb_gain(s, 1);       // 0 = disable , 1 = enable
  s->set_wb_mode(s, 3);        // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
  s->set_exposure_ctrl(s, 0);  // 0 = disable , 1 = enable
  s->set_aec2(s, 0);           // 0 = disable , 1 = enable
  s->set_ae_level(s, 0);       // -2 to 2
  s->set_aec_value(s, 3000);    // 0 to 1200
  s->set_gain_ctrl(s, 0);      // 0 = disable , 1 = enable
  s->set_agc_gain(s, 5);       // 0 to 30
  s->set_gainceiling(s, (gainceiling_t)0);  // 0 to 6
  s->set_bpc(s, 0);            // 0 = disable , 1 = enable
  s->set_wpc(s, 1);            // 0 = disable , 1 = enable
  s->set_raw_gma(s, 1);        // 0 = disable , 1 = enable
  s->set_lenc(s, 1);           // 0 = disable , 1 = enable
  s->set_hmirror(s, 0);        // 0 = disable , 1 = enable
  s->set_vflip(s, 0);          // 0 = disable , 1 = enable
  s->set_dcw(s, 1);            // 0 = disable , 1 = enable
  s->set_colorbar(s, 0);       // 0 = disable , 1 = enable
}

void sendPhotoToServer() {
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  if (LED_Flash_ON) digitalWrite(FLASH_LED_PIN, HIGH);
  delay(1000);

  if (client.connect(serverName.c_str(), serverPort)) {
    Serial.println("Connection successful!");

    String head = "--dataMarker\r\nContent-Disposition: form-data; name=\"imageFile\"; filename=\"ESP32CAMCap.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
    String boundary = "\r\n--dataMarker--\r\n";
    uint32_t imageLen = fb->len;
    uint32_t totalLen = head.length() + boundary.length() + imageLen;

    client.println("POST " + serverPath + " HTTP/1.1");
    client.println("Host: " + serverName);
    client.println("Content-Length: " + String(totalLen));
    client.println("Content-Type: multipart/form-data; boundary=dataMarker");
    client.println();
    client.print(head);

    client.write(fb->buf, fb->len);
    client.print(boundary);

    esp_camera_fb_return(fb);

    int timeout = 10000;
    long startTimer = millis();
    boolean state = false;
    String response = "";

    while ((startTimer + timeout) > millis()) {
      while (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (response.length() == 0) state = true;
          response = "";
        } else if (c != '\r') response += String(c);
        if (state == true) break;
        startTimer = millis();
      }
      if (response.length() > 0) break;
    }
    Serial.println(response);
    client.stop();
  } else {
    Serial.println("Connection to server failed.");
  }

  if (LED_Flash_ON) digitalWrite(FLASH_LED_PIN, LOW);
}

/*...................RTC..............*/
void initializeRTC() {
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1);
    }

    // Uncomment the following line to set the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(__DATE__, __TIME__));

    // Uncomment the following line to set the RTC with an explicit date & time
     rtc.adjust(DateTime(2024, 7, 1, 16, 5, 30)); // Year, Month, Day, Hour, Minute, Second
     Serial.println("RTC updated");
}
/*......function for current time........*/
void printCurrentTime() {
    DateTime now = rtc.now();
    Serial.print("Time: ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
}
void printCurrentDate() {
    DateTime now = rtc.now();
    Serial.print("Date: ");
    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.println();
}
void printDayOfTheWeek() {
    DateTime now = rtc.now();
    Serial.print("Day of the week: ");
    Serial.println(daysOfTheWeek[now.dayOfTheWeek()]);
}


