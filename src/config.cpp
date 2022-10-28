#include <Arduino.h>
#include "config.h"
#include "hardware.h"

#define RPM_BASE 8000

configT config;
uint8_t testMode;
int testValue;

// 30A * 15V = 450W (when holding power) / 20A * 15V = 300W
const unsigned int powerValues[] = { 160, 190, 220, 260, 300, 350, 400, 450 };
//const unsigned int flyTimeValues[] = {60, 180, 300, 360};
// 3:00 default, 5:11 is a basic value, 371 + 25sec = leaves 24sec of 7mins to start timer and to land. 0 stands for run until voltage cut
const unsigned int flyTimeValues[] = {180, 381, 311, 0};

unsigned char i;

void initConfig() {

#ifdef CONFIG_DIP_8

    config.testMode = true;
    for (i=CONFIG_DIP_1; i<=CONFIG_DIP_8; i++) {
        pinMode(i, INPUT_PULLUP);
//        if (digitalRead(i)) {
//            config.testMode = false;
//        }
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
//            - !digitalRead(CONFIG_DIP_1) * 2
//            - !digitalRead(CONFIG_DIP_1) * 4
//            - !digitalRead(CONFIG_DIP_1) * 8
//            - !digitalRead(CONFIG_DIP_1) * 16
//            ;

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

void readTestConfig() {
    i = !digitalRead(CONFIG_DIP_8) * 8 + !digitalRead(CONFIG_DIP_7) * 4 + !digitalRead(CONFIG_DIP_6) * 2 + !digitalRead(CONFIG_DIP_5) * 1;

    if (i>=12) {
        testMode = TEST_MODE_TEST;
    }
    else if (i >= 10) {
        testMode = TEST_MODE_T2_CUT;
    }
    else if (i >= 8) {
        testMode = TEST_MODE_T1_CUT;
    }
    else if (i == 7) {
        testMode = TEST_MODE_MODE;
    }
    else if (i == 6) {
        testMode = TEST_MODE_CURRENT_CUT;
    }
    else if (i == 4) {
        testMode = TEST_MODE_VOLT_CUT;
    }
    else if (i < 4) {
        testMode = TEST_MODE_SMART;
    }
    else {
        testMode = TEST_MODE_UNKNOWN;
    }

    // a switch would take only 4 bytes less of memory
    if (testMode <= TEST_MODE_SMART) {
        testValue = 98
            - readDips(4) * 2
            - !digitalRead(CONFIG_DIP_5) * 20
            - !digitalRead(CONFIG_DIP_6) * 48;

    }
    // 28..90C /2
    else if (testMode <= TEST_MODE_T2_CUT) {
        testValue = 90 - readDips(5) * 2;
    }
    // 3...3.7V / 0.1
    else if (testMode == TEST_MODE_VOLT_CUT) {
        testValue = 7 - readDips(3);
    }
    // 20..50/2
    else if (testMode == TEST_MODE_CURRENT_CUT) {
        testValue = 50 - readDips(4)*2;
    }
    else if (testMode == TEST_MODE_MODE) {
        testValue = readDips(2);
    }
    else {
        testValue = readDips(2);
    }

}
