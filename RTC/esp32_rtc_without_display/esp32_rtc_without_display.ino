#include <Wire.h>
#include "data.h"

void setup() {
    Serial.begin(115200);
    initializeRTC();
}

void loop() {
    printCurrentTime();
    printCurrentDate();
    printDayOfTheWeek();
    delay(1000); // Delay for 1 second
}
