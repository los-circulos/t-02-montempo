#include <Arduino.h>
#include "time.h"
#include "hardware.h"

bool fastBlink() {
    bool ret = millis() / BLINKDIV_ERR % 2 > 0;
    if (ret) {
        ledOn();
    }
    else {
        ledOff();
    }
}
