#include <EEPROM.h>
#include "saved.h"
#include "hardware.h"
#include "config.h"

savedT saved;
uint8_t savedInputMode;
int savedInputValue;

void initSaved() {
    // check if EEPROM has ever been initialized and if not, initialize it
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
        saveSaved();
    }
    else {
        EEPROM.get(ADDR_SAVED, saved);
    }
}
void saveSaved() {
    EEPROM.put(ADDR_SAVED, saved);
}

void readSavedInput() {
//#ifdef INPUT_DIP8
#ifdef ANY_DIP_INPUT
    // DIPS 5...8
    int i = readDips(8) / 16;

    if (i>=12) {
        savedInputMode = SAVED_INPUT_MODE_SPIN;
    }
    else if (i >= 10) {
        savedInputMode = SAVED_INPUT_MODE_T2_CUT;
    }
    else if (i >= 8) {
        savedInputMode = SAVED_INPUT_MODE_T1_CUT;
    }
    else if (i == 7) {
        savedInputMode = SAVED_INPUT_MODE_MODE;
    }
    else if (i == 6) {
        savedInputMode = SAVED_INPUT_MODE_POLES;
    }
//    else if (i == 5) {
//        savedInputMode = SAVED_INPUT_MODE_CURRENT_CUT;
//    }
    else if (i == 4) {
        savedInputMode = SAVED_INPUT_MODE_VOLT_CUT;
    }
//    else {
    else if (i < 4) {
        savedInputMode = SAVED_INPUT_MODE_SMART;
    }
    else {
        savedInputMode = SAVED_INPUT_MODE_CURRENT_CUT;
    }

    // a switch would take only 4 bytes less of memory
    if (savedInputMode <= SAVED_INPUT_MODE_SMART) {
        savedInputValue = 98
                          - readDips(4) * 2
                          - !digitalRead(INPUT_DIP_5) * 20
                          - !digitalRead(INPUT_DIP_6) * 48;
    }
        // 28..90C /2
    else if (savedInputMode <= SAVED_INPUT_MODE_T2_CUT) {
        savedInputValue = 90 - readDips(5) * 2;
    }
        // 3...3.7V / 0.1
    else if (savedInputMode == SAVED_INPUT_MODE_VOLT_CUT) {
        savedInputValue = 7 - readDips(3);
    }
        // 20..50/2
    else if (savedInputMode == SAVED_INPUT_MODE_CURRENT_CUT) {
        savedInputValue = 50 - readDips(4) * 2;
    }
    else if (savedInputMode == SAVED_INPUT_MODE_MODE) {
        savedInputValue = readDips(2);
    }
    else if (savedInputMode == SAVED_INPUT_MODE_POLES){
        savedInputValue = 34 - readDips(4) * 2;
    }
    else {
        savedInputValue = readDips(2);
    }
#endif
}
