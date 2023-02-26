#include <EEPROM.h>
#include "saved.h"
#include "hardware.h"
#include "metrics.h"

savedT saved;
uint8_t savedInputMode;
int savedInputValue;
unsigned int lastFlightNumber = 0;
unsigned char maxLogs;

void initSaved() {
    // check if EEPROM has ever been initialized (with current version) and if not, initialize it
    bool initialized = true;
    int i;
    for (i=0; i<3; i++) {
        if (EEPROM.read(i) != ANSWER_TO_THE_ULTIMATE_QUESTION_OF_LIFE) {
            initialized = false;
            EEPROM.write(i, ANSWER_TO_THE_ULTIMATE_QUESTION_OF_LIFE);
        }
    }
    // fill memory with 0s, who knows what's there :D
    if (!initialized) {
        for (i=EEPROM.length()-1; i>3; i--) {
            EEPROM.write(i, 0);
        }
        saveSavedSetup();
    }
    else {
        EEPROM.get(ADDR_SAVED, saved);
    }

    // this might overflow, which works but might be strange. So first load into an uint then convert
//    maxLogs = (EEPROM.length() - ADDR_LOGS) / sizeof(metricsSum);
    lastFlightNumber = (EEPROM.length() - ADDR_LOGS) / sizeof(metricsSum);
    maxLogs = lastFlightNumber > 200 ? 200 : lastFlightNumber;
    lastFlightNumber = EEPROM.read(ADDR_LAST_FLIGHT_NUM);
    loadMetricsLog();

}
void saveSavedSetup() {
    EEPROM.put(ADDR_SAVED, saved);
}
void readSavedSetup() {

#ifdef ANY_DIP_INPUT

    savedInputMode = readInputLeft();

    // preloading once saves memory over calling the same in various locations
    savedInputValue = readInputRight();

    // lowest bit of savedInputMode is the highest bit of throttle
    if (savedInputMode <= SAVED_INPUT_MODE_SPIN2) {
        savedInputValue = readInputThrottle() - (savedInputMode%2) * 36;
        savedInputMode = SAVED_INPUT_MODE_SPIN;
    }
    // 32..90C /2
    else if (savedInputMode <= SAVED_INPUT_MODE_T2_CUT) {
        savedInputValue = savedInputValue < 30 ? 90 - savedInputValue * 2 : 0;
    }
    // 3...3.7V / 0.1
    else if (savedInputMode == SAVED_INPUT_MODE_VOLT_CUT) {
        savedInputValue = 7 - (savedInputValue % 16) / 2;
    }
    else if (savedInputMode == SAVED_INPUT_MODE_POLES){
        savedInputValue = 34 - savedInputValue * 2;
    }
    else if (savedInputMode == SAVED_INPUT_MODE_END_THROTTLE) {
        savedInputValue = readInputThrottle();
    }
    else if (savedInputMode == SAVED_INPUT_MODE_SOFT_TIME) {
        savedInputValue = 7 - (savedInputValue % 8);
    }
    else if (savedInputMode == SAVED_INPUT_MODE_COUNTDOWN) {
        savedInputValue = 87 - savedInputValue * 2;
    }
    // only 2 modes available
//    else if (savedInputMode == SAVED_INPUT_MODE_MODE) {
//        savedInputValue = (savedInputValue/2) % 4;
//    }
    else {
        savedInputValue = (savedInputValue % 4) % 2;
    }

#ifdef PIN_CURRENT
    // 20..50/2
    else if (savedInputMode == SAVED_INPUT_MODE_CURRENT_CUT_OBS) {
        savedInputValue = savedInputValue <30 ? 50 - savedInputValue : 0;
    }
#endif

#endif
}

void saveMetricsAfterFlight() {
    updateMetricsFlightTime();
    lastFlightNumber++;
    EEPROM.write(ADDR_LAST_FLIGHT_NUM, lastFlightNumber);
    saveMetricsLog();
}
void saveMetricsLog() {
    EEPROM.put(ADDR_LOGS + sizeof(metricsSum) * (lastFlightNumber % maxLogs), metricsSum);
}
void loadMetricsLog() {
    EEPROM.get(ADDR_LOGS + sizeof(metricsSum) * (lastFlightNumber % maxLogs), metricsSum);
}
