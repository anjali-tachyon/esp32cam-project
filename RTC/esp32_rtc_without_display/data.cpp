#include "data.h"
#include <Wire.h>
#include <Arduino.h>

// Instantiating the RTC Module
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void initializeRTC() {
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1);
    }

    // Uncomment the following line to set the RTC to the date & time this sketch was compiled
     rtc.adjust(DateTime(__DATE__, __TIME__));

    // Uncomment the following line to set the RTC with an explicit date & time
     rtc.adjust(DateTime(2024, 6, 7, 12, 0, 0)); // Year, Month, Day, Hour, Minute, Second
}

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