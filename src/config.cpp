#include <Arduino.h>
#include "config.h"
#include "hardware.h"

#define RPM_BASE 8000

configT config;

// 30A * 15V = 450W (when holding power) / 20A * 15V = 300W
//const unsigned int powerValues[] = { 160, 190, 220, 260, 300, 350, 400, 450 };
const unsigned int powerValues[] = { 120, 140, 160, 180, 200, 220, 240, 260, 280, 300, 330, 360, 390, 420, 450, 480 };
// 3:00 default, 370 + 25sec = leaves 25sec of the 7mins to start timer and to land. 0 stands for run until voltage cut
#ifdef DEVMODE
const unsigned int flyTimeValues[] = { 10, 60, 240, 300, 340, 355, 370, 0 };
//const unsigned int flyTimeValues[] = { 10, 60, 20, 30, 40, 45, 50, 55 };
#else
const unsigned int flyTimeValues[] = { 180, 60, 240, 300, 340, 355, 370, 0 };
#endif

unsigned char i;

// abstracted so I could reuse code when viewing logs? Seems it gets inline-optimized meanwhile :D
void setHoldValues(unsigned char rawHoldValue) {
    config.holdValueRaw = rawHoldValue;
    config.holdThrottle = 98 - rawHoldValue * 2;
    config.holdRPM = RPM_BASE + rawHoldValue * 300;
    config.holdPower = powerValues[rawHoldValue];
}

void initConfig() {

#ifdef INPUT_DIP_8

    config.savedInputMode = true;
    for (i=INPUT_DIP_1; i <= INPUT_DIP_8; i++) {
        pinMode(i, INPUT_PULLUP);
        if (digitalRead(i)) {
            config.savedInputMode = false;
        }
    }

#endif

}
void readConfigInput() {

#ifdef INPUT_DIP8
    i = readDips(4);
    setHoldValues(i);

    i = (readDips(7) - i) / 16;
    config.timeFly = flyTimeValues[i];

    // @todo screen rotate delay should be read from eprom (?)
    // @todo default screen should be read from eprom

    // @todo this value should be refreshed in countdown and restart countdown if changes
    config.rotateScreens = !digitalRead(INPUT_DIP_8);
#endif

}
