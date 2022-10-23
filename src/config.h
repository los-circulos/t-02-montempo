#ifndef MONTEMPO_CONFIG_H
#define MONTEMPO_CONFIG_H

struct configT {
    unsigned int throttle = 80;
    unsigned long RPM = 0;
    bool holdRPM = false;
    unsigned int current = 0;
    bool holdCurrent = false;
    unsigned int timeDelay = 25;
    unsigned int timeFly = 180;
    /** which screen to show */
    int screen = 0;
    bool rotateScreens = true;
    int screenRotateTime = 2;
    bool testMode = false;
};

extern configT config;

void initConfig();
void readConfig();

#endif //MONTEMPO_CONFIG_H
