
#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

char days[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
void setup();

void setup() {
  Serial.begin(115200);

  if (!rtc.begin()) {
    Serial.println("Could not find RTC! Check circuit.");
    while (1);
  }

  rtc.adjust(DateTime(__DATE__, __TIME__));
}

void loop() {
  DateTime now = rtc.now();

  Serial.print("Time: ");
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  Serial.print(now.minute(), DEC);
  Serial.print(":");
  Serial.println(now.second(), DEC);

  Serial.print("Date: ");
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(days[now.dayOfTheWeek()]);
  Serial.print(" ");
  Serial.print(now.month(), DEC);
  Serial.print(" ");
  Serial.println(now.year(), DEC);
  Serial.println("Debug message");
  delay(1000); // Adjust delay as needed
}
