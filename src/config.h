#ifndef MONTEMPO_CONFIG_H
#define MONTEMPO_CONFIG_H

#define TEST_MODE_TEST 0
#define TEST_MODE_SMART 1
#define TEST_MODE_T1_CUT 2
#define TEST_MODE_T2_CUT 3
#define TEST_MODE_VOLT_CUT 4
#define TEST_MODE_CURRENT_CUT 5
#define TEST_MODE_MODE 6
#define TEST_MODE_UNKNOWN 7

#define THROTTLE_CONST 0
#define THROTTLE_HOLD_RPM 1
#define THROTTLE_HOLD_POWER 2
#define THROTTLE_HOLD_SMART 3

// @todo this is a runtime setup, not config
struct configT {
    unsigned int throttle = 80;
    bool smartThrottle = false;
    unsigned int smartEndThrottle = 95;
    unsigned int RPM = 0;
    bool holdRPM = false;
    unsigned int power = 300;
    bool holdPower = false;
    bool runUntilCutoff = false;
//    unsigned int timeDelay = 25;
    unsigned int timeDelay = 3;
//    unsigned int softStartTime = 5;
    unsigned int softStartTime = 0;
    unsigned int timeFly = 180;
//    unsigned int cutoffVoltage = 36;
    float cutoffVoltage = 3.6;
    /** which screen to show */
    int defaultScreen = 0;
    bool rotateScreens = true;
    int screenRotateTime = 2;
    bool testMode = false;
    bool btnAEnabled = true;
    bool btnBEnabled = true;
};

extern configT config;

extern unsigned char testMode;
extern int testValue;

void initConfig();
void readConfig();
void readTestConfig();

#endif //MONTEMPO_CONFIG_H
