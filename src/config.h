#ifndef MONTEMPO_CONFIG_H
#define MONTEMPO_CONFIG_H

#define TESTMODE_SPIN 0
#define TESTMODE_SMART 1
#define TESTMODE_T1_CUT 2
#define TESTMODE_T2_CUT 3
#define TESTMODE_VOLT_CUT 4
#define TESTMODE_CURRENT_CUT 5
#define TESTMODE_MODE 6
#define TESTMODE_POLES 7
// todo write logs, and write log browser with this mode
#define TESTMODE_LOGS 7

// @todo this is a runtime setup, not config
struct configT {
    bool testMode = false;
    bool btnAEnabled = true;
    bool btnBEnabled = true;

    unsigned int throttle = 80;
    bool smartThrottle = false;
    unsigned int RPM = 0;
    bool holdRPM = false;
    unsigned int power = 300;
    bool holdPower = false;
    bool runUntilCutoff = false;
//    unsigned int timeDelay = 25;
//    unsigned int timeDelay = 10;
    unsigned int timeDelay = 10;

//    unsigned int softStartTime = 5;
    unsigned int softStartTime = 0;

    unsigned int timeFly = 180;
//    float cutoffVoltage = 3.7;
    /** which screen to show */
    int defaultScreen = 0;
    bool rotateScreens = true;
    int screenRotateTime = 2;
};

extern configT config;

extern unsigned char testMode;
extern int testValue;

void initConfig();
void readConfig();
void readTestConfig();

#endif //MONTEMPO_CONFIG_H
