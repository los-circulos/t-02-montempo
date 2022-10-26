#include <Arduino.h>
#include "mytime.h"
#include "hardware.h"

unsigned long currentTime;

void setCurrentTime() {
    currentTime = millis();
}

bool blinkLed(uint8_t t) {
    bool ret = (currentTime / t /10) % 2 > 0;
    if (ret) {
        ledOn();
    }
    else {
        ledOff();
    }
    return ret;
}
