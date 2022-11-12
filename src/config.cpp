#include <Arduino.h>
#include "config.h"
#include "hardware.h"

#define RPM_BASE 8000

configT config;
uint8_t testMode;
int testValue;

// 30A * 15V = 450W (when holding power) / 20A * 15V = 300W
//const unsigned int powerValues[] = { 160, 190, 220, 260, 300, 350, 400, 450 };
const unsigned int powerValues[] = { 120, 140, 160, 180, 200, 220, 240, 260, 280, 300, 330, 360, 390, 420, 450, 480 };
// 3:00 default, 370 + 25sec = leaves 25sec of the 7mins to start timer and to land. 0 stands for run until voltage cut
const unsigned int flyTimeValues[] = { 180, 60, 240, 300, 340, 355, 370, 0 };

unsigned char i;

void initConfig() {

#ifdef CONFIG_DIP_8

    config.testMode = true;
    for (i=CONFIG_DIP_1; i<=CONFIG_DIP_8; i++) {
        pinMode(i, INPUT_PULLUP);
        if (digitalRead(i)) {
            config.testMode = false;
        }
    }

#endif

}

void readConfig() {

#ifdef CONFIG_DIP_8

    config.throttle = 98 - readDips(4) * 2;

    config.RPM = RPM_BASE + readDips(4) * 300;

    config.power = powerValues[readDips(4)];

    i = (readDips(7) - readDips(4)) / 16;
    config.timeFly = flyTimeValues[i];

    // @todo screen rotate delay should be read from eprom (?)
    // @todo default screen should be read from eprom

    // @todo this value should be refreshed in countdown and restart countdown if changes
    config.rotateScreens = !digitalRead(CONFIG_DIP_8);

#endif

}

void readTestConfig() {
    i = !digitalRead(CONFIG_DIP_8) * 8 + !digitalRead(CONFIG_DIP_7) * 4 + !digitalRead(CONFIG_DIP_6) * 2 + !digitalRead(CONFIG_DIP_5) * 1;

    if (i>=12) {
        testMode = TESTMODE_SPIN;
    }
    else if (i >= 10) {
        testMode = TESTMODE_T2_CUT;
    }
    else if (i >= 8) {
        testMode = TESTMODE_T1_CUT;
    }
    else if (i == 7) {
        testMode = TESTMODE_MODE;
    }
    else if (i == 6) {
        testMode = TESTMODE_POLES;
    }
    else if (i == 4) {
        testMode = TESTMODE_VOLT_CUT;
    }
    else if (i < 4) {
        testMode = TESTMODE_SMART;
    }
    else {
        testMode = TESTMODE_CURRENT_CUT;
    }

    // a switch would take only 4 bytes less of memory
    if (testMode <= TESTMODE_SMART) {
        testValue = 98
            - readDips(4) * 2
            - !digitalRead(CONFIG_DIP_5) * 20
            - !digitalRead(CONFIG_DIP_6) * 48;
    }
    // 28..90C /2
    else if (testMode <= TESTMODE_T2_CUT) {
        testValue = 90 - readDips(5) * 2;
    }
    // 3...3.7V / 0.1
    else if (testMode == TESTMODE_VOLT_CUT) {
        testValue = 7 - readDips(3);
    }
    // 20..50/2
    else if (testMode == TESTMODE_CURRENT_CUT) {
        testValue = 50 - readDips(4)*2;
    }
    else if (testMode == TESTMODE_MODE) {
        testValue = readDips(2);
    }
    else if (testMode == TESTMODE_POLES){
        testValue = 34 - readDips(4) * 2;
    }
    else {
        testValue = readDips(2);
    }
}
