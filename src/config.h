#ifndef MONTEMPO_CONFIG_H
#define MONTEMPO_CONFIG_H

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

void initConfig();
void readConfig();

#endif //MONTEMPO_CONFIG_H
