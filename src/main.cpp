#include <Arduino.h>
#include "config.h"
#include "hardware.h"
#include "main.h"
#include "time.h"
#include <screen.h>

uint8_t currentMode = MODE_WELCOME_LOCK;
unsigned long currentModeStarted = 0;
// elapsed incremental cycles since last mode change. !! use only through elapsedInMode().
unsigned long elapsedInModeCounter = 0;

unsigned long m;

void setMode(int newMode);
bool elapsedInMode(unsigned int);

void setup() {

    initConfig();
    initScreen();

    readConfig();

    drawWelcome();
}

void loop() {

    m = millis();
    int i;

    switch (currentMode) {
        case MODE_WELCOME_LOCK:
            // @todo check if any button is enabled
            if (!ANY_BUTTON_PRESSED) {
                setMode(MODE_WELCOME_COUNTDOWN);
                eraseLogoLock();
            }
            else {
                drawLogoLock();
            }
            break;
        case MODE_WELCOME_COUNTDOWN:
            if (ANY_BUTTON_PRESSED) {
                setMode(MODE_WELCOME_LOCK);
            }
            else if (elapsedInModeCounter > 6) {
                if (config.testMode) {
                    setMode(MODE_TEST);
                }
                else {
                    setMode(MODE_CONFIG);
                }
            }
            else if (elapsedInMode(DELAY_COUNTDOWN)) {
                drawWaitDot(elapsedInModeCounter);
            }
            break;
        case MODE_TEST:

            break;
        case MODE_CONFIG:
            if (elapsedInMode(200)) {
                // @todo time this so we can see if it interferes with servo signal at all
                readConfig();
                drawScreen(config);

                if (btnBDisabled() || btnBPushed()) {
                    ledOn();
                    if (btnADisabled() || btnAPushed()) {
                        setMode(MODE_CONFIG_COUNTDOWN);
                    }
                    if (elapsedInModeCounter % 2 > 0) {
                        drawFlyConfirmation(true);
                    }
                    else {
                        drawFlyConfirmation(false);
                    }
                }

            }

            break;
        case MODE_CONFIG_COUNTDOWN:
            if ((!btnADisabled() && !btnAPushed()) || (!btnBDisabled() && !btnBPushed())) {
                setMode(MODE_CONFIG);
                drawFlyConfirmation(false);
            }
            else if (elapsedInModeCounter > 6) {
                setMode(MODE_DELAY);
            }
            else if (elapsedInMode(DELAY_COUNTDOWN)) {
                drawWaitDot(elapsedInModeCounter);
            }
            fastBlink();
            break;
        case MODE_DELAY:
            break;
        case MODE_FLY:
            break;
        case MODE_AFTER:
            break;
    }

}

void setMode(int newMode) {
    currentMode = newMode;
    currentModeStarted = m;
    elapsedInModeCounter = 0;
    switch (newMode) {
        case MODE_CONFIG:
            clearScreen();
        break;
        case MODE_CONFIG_COUNTDOWN:
            ledOn();
        break;
    }
}

/**
 * A tricky stepper for "incremental ticks"
 * eg. a waiting bar from . to ...... is implemented in MODE_WELCOME_COUNTDOWN. It uses the return value
 *  and if 250 millis have elapsed since drawing the last dot, it will draw another dot.
 * @param elapsedMillis
 */
bool elapsedInMode(unsigned int elapsedMillis) {
    unsigned long elapsedCnt = (m - currentModeStarted) / elapsedMillis;
    if (elapsedCnt > elapsedInModeCounter) {
        elapsedInModeCounter = elapsedCnt;
        return true;
    }
    return false;
}
