#ifndef MONTEMPO_SAVED_H
#define MONTEMPO_SAVED_H

#define ANSWER_TO_THE_ULTIMATE_QUESTION_OF_LIFE 42

#define ADDR_SAVED 4
#define ADDR_LOGS 100

#define HOLD_MODE_FLAT_THROTTLE 0
#define HOLD_MODE_RPM 1
#define HOLD_MODE_POWER 2
#define HOLD_MODE_SMART_THROTTLE 3

struct savedT {
//    unsigned int k = 720;
    // the default is very low so if RPM hold is attempted without setting up, it will just underdrive the motor
    unsigned char poles = 4;
    unsigned char holdMode = HOLD_MODE_FLAT_THROTTLE;
    unsigned char smartEndThrottle = 90;
    unsigned char voltCut = 7;
    unsigned char currentCut = 50;
    unsigned char t1Cut = 90;
    unsigned char t2Cut = 90;
};

extern savedT saved;

void initSaved();
void saveSaved();

#endif //MONTEMPO_SAVED_H
