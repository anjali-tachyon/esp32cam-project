#ifndef DATA_H
#define DATA_H

#include <RTClib.h>

// Function declarations
void initializeRTC();
void printCurrentTime();
void printCurrentDate();
void printDayOfTheWeek();

// External variable declarations
extern RTC_DS3231 rtc;
extern char daysOfTheWeek[7][12];

#endif 