#ifndef MONTEMPO_SAVED_H
#define MONTEMPO_SAVED_H

#define ANSWER_TO_THE_ULTIMATE_QUESTION_OF_LIFE 42

#define ADDR_SAVED 4
#define ADDR_LOGS 100

#define HOLD_MODE_FLAT_THROTTLE 0
#define HOLD_MODE_RPM 1
#define HOLD_MODE_POWER 2
#define HOLD_MODE_SMART_THROTTLE 3

#define SAVED_INPUT_MODE_SPIN 0
#define SAVED_INPUT_MODE_SMART 1
#define SAVED_INPUT_MODE_T1_CUT 2
#define SAVED_INPUT_MODE_T2_CUT 3
#define SAVED_INPUT_MODE_VOLT_CUT 4
#define SAVED_INPUT_MODE_CURRENT_CUT 5
#define SAVED_INPUT_MODE_MODE 6
#define SAVED_INPUT_MODE_POLES 7
// todo write logs, and write log browser with this mode
#define SAVED_INPUT_MODE_LOGS 7

struct savedT {
    unsigned char smartEndThrottle = 90;
    unsigned char t1Cut = 90;
    unsigned char t2Cut = 90;
    unsigned char holdMode = HOLD_MODE_FLAT_THROTTLE;
    // the default is very low so if RPM hold is attempted without setting up, it will just underdrive the motor
    unsigned char poles = 4;
    // 3.7
    unsigned char voltCut = 7;
    unsigned char currentCut = 30;
};

extern savedT saved;
extern unsigned char savedInputMode;
extern int savedInputValue;

void initSaved();
void saveSaved();

void readSavedInput();

#endif //MONTEMPO_SAVED_H
