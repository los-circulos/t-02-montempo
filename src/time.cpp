#include <Arduino.h>
#include "time.h"

unsigned long currentTime;

bool errorBlink() {
    return millis() / BLINKDIV_ERR % 2 > 0;
}
