#ifndef MONTEMPO_CONFIG_H
#define MONTEMPO_CONFIG_H

#define DEVMODE

#define CONFIG_CELLS_MAX 5
#define CONFIG_CELLS_ERR_VCUT 6
#define CONFIG_CELLS_ERR_VLOW 7

// @todo this is a runtime setup, not config
struct configT {
    bool savedInputMode = false;

    unsigned int holdThrottle = 80;
    unsigned int holdRPM = 0;
    unsigned int holdPower = 300;
    unsigned char holdValueRaw = 0;
#ifdef DEVMODE
    unsigned int timeDelay = 3;
#else
    unsigned int timeDelay = 25;
#endif

    unsigned int timeFly = 180;
//    float cutoffVoltage = 3.7;
    /** which screen to show */
    int defaultScreen = 0;
    bool rotateScreens = true;
    int screenRotateTime = 5;

    unsigned int maxRPMs = 15000;

    unsigned char cellCount = 0;
    bool preflightError = false;
};

extern configT config;

void initConfig();
void readConfigInput();

#endif //MONTEMPO_CONFIG_H
