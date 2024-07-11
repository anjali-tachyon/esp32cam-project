#ifndef DEEPSLEEP_H
#define DEEPSLEEP_H
#define uS_TO_S_FACTOR 1000000

#include <RTClib.h>
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_VL53L0X.h"

void initializeRTC();
void printCurrentTime();
void printCurrentDate();
void printDayOfTheWeek();

#endif
