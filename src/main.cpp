#include <Arduino.h>
#include "config.h"
#include "hardware.h"
#include "main.h"
#include "mytime.h"
#include "saved.h"
#include <screen.h>

uint8_t currentMode = MODE_WELCOME_LOCK;
//uint8_t currentMode = MODE_CONFIG;
//uint8_t currentMode = MODE_DELAY;
unsigned long currentModeStarted = 0;
// elapsed incremental cycles since last mode change. !! use only through elapsedInMode().
unsigned long elapsedInModeCounter = 0;

void notImplemented();
void setMode(int newMode);
bool elapsedInMode(unsigned int);
void confirmation();
void countDown(char nextMode);

void setup() {

//    Serial.begin(9600);

    initSaved();
    initConfig();
    initScreen();

    readConfig();

    drawWelcome();

}

void loop() {

    setCurrentTime();
    unsigned int i;

    switch (currentMode) {
        case MODE_WELCOME_LOCK:
            // @todo during welcome lock I should show previous flight values
            if (!ANY_BUTTON_PUSHED) {
                setMode(MODE_WELCOME_COUNTDOWN);
                eraseLogoLock();
            }
            else {
                drawLogoLock();
            }
            break;
        case MODE_WELCOME_COUNTDOWN:
            if (ANY_BUTTON_PUSHED) {
                setMode(MODE_WELCOME_LOCK);
            }
            else if (elapsedInModeCounter > 6) {
                if (config.testMode) {
                    setMode(MODE_TEST);
                }
                else {
                    setMode(MODE_CONFIG);
                }
//                    setMode(MODE_TEST);
            }
            else if (elapsedInMode(DELAY_COUNTDOWN)) {
                drawWaitDot(elapsedInModeCounter);
            }
            break;
        case MODE_TEST:
            // @todo deny test/setup if both buttons are disabled
            if (elapsedInMode(200)) {
                readTestConfig();
                drawTestScreen();
                confirmation();
            }
            break;
        case MODE_TEST_COUNTDOWN:
//            countDown(MODE_TEST_RUN);
            countDown(MODE_TEST_SAVE);
            break;
        case MODE_TEST_SAVE:
            if (testMode == TESTMODE_SPIN) {
                setMode(MODE_TEST_RUN);
                return;
            }
            else if (testMode == TESTMODE_SMART) {
                saved.smartEndThrottle = testValue;
            }
            else if (testMode == TESTMODE_T1_CUT) {
                saved.t1Cut = testValue;
            }
            else if (testMode == TESTMODE_T2_CUT) {
                saved.t2Cut = testValue;
            }
            else if (testMode == TESTMODE_VOLT_CUT) {
                saved.voltCut = testValue;
            }
            else if (testMode == TESTMODE_CURRENT_CUT) {
                saved.currentCut = testValue;
            }
            else if (testMode == TESTMODE_MODE) {
                saved.holdMode = testValue;
            }
            saveSaved();
            break;
        case MODE_TEST_SAVED:
            if (elapsedInMode(200)) {
                if ((elapsedInModeCounter > 10) && !ANY_BUTTON_PUSHED) {
                    setMode(MODE_TEST);
                }
                drawSaved();
            }
            break;
        case MODE_TEST_RUN:
            drawRunScreen();
            break;
        case MODE_CONFIG:
            if (elapsedInMode(200)) {

                readConfig();
                drawScreen(config);
                confirmation();
            }
            break;
        case MODE_CONFIG_COUNTDOWN:
            countDown(MODE_DELAY_LOCK);
            break;
        case MODE_DELAY_LOCK:
            if (!ANY_BUTTON_PUSHED) {
                setMode(MODE_DELAY);
                eraseLogoLock();
            }
            else if (elapsedInMode(100)) {
                drawLogoLock();
            }
            break;
        case MODE_DELAY:
            if (ANY_BUTTON_PUSHED) {
                setMode(MODE_CONFIG);
            }
            // NOTE this has to be in sync with BLINK_FAST blinking, hence the high rate
            if (elapsedInMode(100)) {
                drawRemainingTime(config.timeDelay - (currentTime - currentModeStarted) / 1000);
            }
            // we increment elapsedInModeCounter twice a sec, so we divide by 2
            if (elapsedInModeCounter / 10 >= config.timeDelay) {
                setMode(MODE_FLY);
            }
            break;
        // @todo soft start?
        case MODE_SOFT_START:
            break;
        case MODE_FLY:
            if (config.holdRPM || config.holdPower || config.smartThrottle) {
                notImplemented();
                return;
            }
            // simple throttle
            else {
                throttlePcnt(config.throttle);
            }
            ledOn();
            if (elapsedInMode(100)) {
                // draw remaining / elapsed time
                i = (currentTime - currentModeStarted) / 1000;
                // remaining time is flight time minus soft start time (elapsed already) minus elapsed time
                if (!config.runUntilCutoff) {
                    i = config.timeFly - config.softStartTime - i;
                }
                // when using soft start and incremental time, add it to elapsed time (twice to increment previous deduction)
                else {
                    i+= 2*config.softStartTime;
                }
                drawRemainingTime(i);
                if (config.runUntilCutoff) {
                    ledOff();
                }
            }
            break;
        case MODE_AFTER:
            break;
    }

}

void notImplemented() {
    throttleOff();
    if (elapsedInMode(100)) {
        drawNotImplemented();
    }
}

void setMode(int newMode) {
    currentMode = newMode;
    currentModeStarted = currentTime;
    elapsedInModeCounter = 0;
    switch (newMode) {
        case MODE_CONFIG:
            clearScreen();
            // interestingly, if I remove the break and the then duplicate clearScreen, it uses more memory
        break;
        case MODE_DELAY_LOCK:
        case MODE_TEST_RUN:
            clearScreen();
        break;
        case MODE_TEST:
        case MODE_DELAY:
            throttle.attach(PIN_THROTTLE);
            throttleOff();
            clearScreen();
        break;
    }
    ledOff();
}

/**
 * A tricky stepper for "incremental ticks"
 * eg. a waiting bar from . to ...... is implemented in MODE_WELCOME_COUNTDOWN. It uses the return value
 *  and if 250 millis have elapsed since drawing the last dot, it will draw another dot.
 * @param elapsedMillis
 */
bool elapsedInMode(unsigned int elapsedMillis) {
    unsigned long elapsedCnt = (currentTime - currentModeStarted) / elapsedMillis;
    if (elapsedCnt > elapsedInModeCounter) {
        elapsedInModeCounter = elapsedCnt;
        return true;
    }
    return false;
}

void confirmation() {
    if (btnBDisabled() || btnBPushed()) {
        if (btnADisabled() || btnAPushed()) {
            setMode(currentMode + 1);
        }
        if (elapsedInModeCounter % 2 > 0) {
            drawFlyConfirmation(true);
            ledOn();
        }
        else {
            drawFlyConfirmation(false);
            ledOff();
        }
    }
    else {
        drawFlyConfirmation(false);
        ledOff();
    }
}

void countDown(char nextMode) {
    if ((!btnADisabled() && !btnAPushed()) || (!btnBDisabled() && !btnBPushed())) {
        setMode(currentMode - 1);
        drawFlyConfirmation(false);
    }
    else if (elapsedInModeCounter > 6) {
        setMode(nextMode);
    }
    else if (elapsedInMode(DELAY_COUNTDOWN)) {
        drawFlyConfirmation(true);
        drawWaitDot(elapsedInModeCounter);
    }
    blinkLed(BLINK_FAST);
}