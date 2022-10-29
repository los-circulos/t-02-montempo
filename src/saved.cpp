#include <EEPROM.h>
#include "saved.h"

savedT saved;

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
