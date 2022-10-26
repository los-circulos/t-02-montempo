#include <Arduino.h>
#include "config.h"
#include "hardware.h"

#define RPM_BASE 8000

configT config;
//const unsigned int currentValues[] = {13,18,21,24,28,33,37,45};
// 30A * 15V = 450W (when holding power) / 20A * 15V = 300W
const unsigned int powerValues[] = { 160, 190, 220, 260, 300, 350, 400, 450 };
//const unsigned int flyTimeValues[] = {60, 180, 300, 360};
// 3:00 default, 5:11 is a basic value, 371 + 25sec = leaves 24sec of 7mins to start timer and to land. 0 stands for run until voltage cut
const unsigned int flyTimeValues[] = {180, 381, 311, 0};

uint8_t i;

void initConfig() {

#ifdef CONFIG_DIP_8

    config.testMode = true;
    for (i=CONFIG_DIP_1; i<=CONFIG_DIP_8; i++) {
        pinMode(i, INPUT_PULLUP);
        if (digitalRead(i)) {
            config.testMode = false;
        }
    }

    pinMode(LED1, OUTPUT);

    // NOTE on the nano these won't work with default pins A6, A7 and these two pins need a pullup resistor
    if (btnADisabled()) {
        config.btnAEnabled = false;
    }
    else {
        pinMode(BTN_A, INPUT_PULLUP);
    }
    if (btnBDisabled()) {
        config.btnBEnabled = false;
    }
    else {
        pinMode(BTN_B, INPUT_PULLUP);
    }

#endif

}

void readConfig() {

//#ifdef CONFIG_JMP_4
//    // not implemented atm
//#endif

#ifdef CONFIG_DIP_8

    config.throttle = 98
            - !digitalRead(CONFIG_DIP_1) * 13
            - !digitalRead(CONFIG_DIP_2) * 7
            - !digitalRead(CONFIG_DIP_3) * 3;

    config.RPM = RPM_BASE
            + !digitalRead(CONFIG_DIP_1) * 2000
            + !digitalRead(CONFIG_DIP_2) * 1000
            + !digitalRead(CONFIG_DIP_3) * 500;
    // reach 12000 max rpm
    if (config.RPM == RPM_BASE + 3500) {
        config.RPM += 500;
    }

    i = !digitalRead(CONFIG_DIP_1) * 4
            + !digitalRead(CONFIG_DIP_2) * 2
            + !digitalRead(CONFIG_DIP_3) * 1;
    config.power = powerValues[i];

    i = !digitalRead(CONFIG_DIP_4) * 2
        + !digitalRead(CONFIG_DIP_5) * 1;
    config.timeFly = flyTimeValues[i];
    if (config.timeFly == 0) {
        config.runUntilCutoff = true;
    }

    config.holdRPM = !digitalRead(CONFIG_DIP_6) && digitalRead(CONFIG_DIP_7);

    config.holdPower = digitalRead(CONFIG_DIP_6) && !digitalRead(CONFIG_DIP_7);

    config.smartThrottle = !digitalRead(CONFIG_DIP_6) && !digitalRead(CONFIG_DIP_7);

    // @todo rotate delay should be read from eprom
    // @todo default screen should be read from eprom
    // @todo this value should be refreshed periodically
    config.rotateScreens = !digitalRead(CONFIG_DIP_8);

#endif

}
