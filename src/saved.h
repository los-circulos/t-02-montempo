#ifndef MONTEMPO_SAVED_H
#define MONTEMPO_SAVED_H

#define ANSWER_TO_THE_ULTIMATE_QUESTION_OF_LIFE 42

#define ADDR_SAVED 4
#define ADDR_LAST_FLIGHT_NUM 50
#define ADDR_LOGS 60

#define HOLD_MODE_HOLD_THROTTLE 0
#define HOLD_MODE_SMART_THROTTLE 1
#define HOLD_MODE_RPM 2
#define HOLD_MODE_POWER 3

#define SAVED_INPUT_MODE_SPIN 0
#define SAVED_INPUT_MODE_SPIN2 1
#define SAVED_INPUT_MODE_T1_CUT 2
#define SAVED_INPUT_MODE_T2_CUT 3
#define SAVED_INPUT_MODE_MODE 4
#define SAVED_INPUT_MODE_VOLT_CUT 5
#define SAVED_INPUT_MODE_POLES 6
#define SAVED_INPUT_MODE_END_THROTTLE 7
#define SAVED_INPUT_MODE_GOVI 8
#define SAVED_INPUT_MODE_ARM 9
#define SAVED_INPUT_MODE_CAL 10
#define SAVED_INPUT_MODE_SOFT_TIME 11
#define SAVED_INPUT_MODE_COUNTDOWN 12
#define SAVED_INPUT_MODE_CLEAR_LOGS 13
#define SAVED_INPUT_MODE_USERTIME 14
#define SAVED_INPUT_MODE_NOTUSED 15

#define SAVED_ARM_ON_BOOT false
#define SAVED_ARM_ON_DELAY true

#define SAVED_USERTIME_MAX 59

struct savedT {
    unsigned char endValue = 90;
    unsigned char t1Cut = 90;
    unsigned char t2Cut = 90;
    unsigned char holdMode = HOLD_MODE_HOLD_THROTTLE;
    // the default is very low so if RPM hold is attempted without setting up, it will just underdrive the motor
    unsigned char poles = 4;
    // 3.6
    unsigned char voltCut = 6;
#ifdef PIN_CURRENT
    unsigned char currentCut = 30;
#endif
    unsigned char softTime = 3;
    unsigned char countdown = 25;
    unsigned char userTime = 84;
    bool govi = false;
    bool armOnBoot = SAVED_ARM_ON_BOOT;
    bool calibrate = true;
};

extern savedT saved;
extern unsigned char savedInputMode;
extern int savedInputValue;
extern unsigned int lastFlightNumber;
#define CURRENT_FLIGHT_NUMBER (lastFlightNumber+1)

void initSaved();

void saveSavedSetup();
void readSavedSetup();

void saveMetricsAfterFlight();
void saveMetricsLog();
void loadMetricsLog();

#endif //MONTEMPO_SAVED_H
