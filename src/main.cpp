#include <Arduino.h>
#include "config.h"
#include "hardware.h"
#include "main.h"
#include "mytime.h"
#include "saved.h"
#include "metrics.h"
#include <screen.h>

//#define AFTER_SCREEN_DEBUG
//#define RPM_DEBUG

uint8_t currentMode = MODE_WELCOME_LOCK;
unsigned long currentModeStarted = 0;
// elapsed incremental cycles since last mode change. !! use only through elapsedInMode().
unsigned long elapsedInModeCounter = 0;

#ifdef RPM_DEBUG
int xPcnt = 5;
volatile unsigned int rpmCnt = 0;
void rpmISR() {
    rpmCnt++;
}
#endif

#define NOT_IMPLEMENTED (saved.holdMode != HOLD_MODE_FLAT_THROTTLE)
void notImplemented();
void setMode(int newMode);
bool elapsedInMode(unsigned int);
void confirmation();
void countDown(char nextMode);

void setup() {

//    Serial.begin(9600);

    initSaved();
    initConfig();
    initHardware();
    initScreen();

    readConfig();

    drawWelcome();

#ifdef RPM_DEBUG
    delay(2000);
    armThrottle();
    delay(2000);
    throttlePcnt(xPcnt);
#endif

#ifdef PIN_RPM
    attachInterrupt(digitalPinToInterrupt(PIN_RPM), rpmISR, RISING);
#endif

    setMode(MODE_WELCOME_LOCK);

    u8x8.setFont(FONT_L);

}

#ifdef AFTER_SCREEN_DEBUG
unsigned int j = 0;

void loop() {

    if (btnAPushed()) {
        j++;
        clearScreen();
    }
    else if (btnBPushed()) {
        j = max(0, j-1);
        clearScreen();
    }
    if (j>10) {
        j = 0;
    }

    drawAfterScreen(j);

    delay(200);

}
#else
#ifdef RPM_DEBUG
char bufe[20];
void loop() {

    setCurrentTime();
//    unsigned int i;

    if (elapsedInMode(1000)) {
        if (btnAPushed()) {
            xPcnt++;
        }
        else if (btnBPushed()) {
            xPcnt--;
        }
        sprintf(bufe, "%15u", rpmCnt);
        u8x8.drawString(0, 0, bufe);
//        sprintf(bufe, "%15d", ((int)(cc / 30)));
        sprintf(bufe, "%15d ", xPcnt);
        u8x8.drawString(0, 2, bufe);
        throttlePcnt(xPcnt);
//        cc = 0;
        rpmCnt = 0;
    }
    return;

}
#else

void loop() {

    setCurrentTime();
    unsigned int i;

    switch (currentMode) {
        case MODE_WELCOME_LOCK:
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
            else if (elapsedInModeCounter > 8) {
                if (config.testMode) {
                    setMode(MODE_TEST);
                }
                else {
                    setMode(MODE_PREFLIGHT_PROGRAM);
                }
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
            countDown(MODE_TEST_SAVE);
            break;
        case MODE_TEST_SAVE:
            if (testMode == TESTMODE_SPIN) {
                setMode(MODE_TEST_SPIN);
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
            else if (testMode == TESTMODE_POLES) {
                saved.poles = testValue;
            }
            saveSaved();
            setMode(MODE_TEST_SAVED);
            break;
        case MODE_TEST_SAVED:
            if (elapsedInMode(100)) {
                if (ANY_BUTTON_PUSHED) {
                    drawLogoLock();
                    ledOn();
                    currentModeStarted = currentTime;
                    elapsedInModeCounter = 0;
                }
                else if (elapsedInModeCounter > 8) {
                    setMode(MODE_TEST);
//                    return;
                }
                else {
                    drawWaitDot(elapsedInModeCounter);
                }
            }
            break;
        case MODE_TEST_SPIN:
            if (elapsedInMode(200)) {
                readTestConfig();
                readMetrics();
                sumMetrics();
                if ((testMode != TESTMODE_SPIN) || !ANY_BUTTON_PUSHED) {
                    setMode(MODE_WELCOME_LOCK);
                    throttlePcnt(0);
                }
                else {
                    drawRunScreen();
                    throttlePcnt(testValue);
                }
            }
            break;
        case MODE_PREFLIGHT_PROGRAM:
            if (elapsedInMode(200)) {
                readConfig();
                readMetrics();
                drawPreflight(config);
                confirmation();
            }
            break;
        case MODE_PREFLIGHT_PROGRAM_COUNTDOWN:
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
            if (NOT_IMPLEMENTED) {
                notImplemented();
                return;
            }
            else if (ANY_BUTTON_PUSHED) {
                setMode(MODE_PREFLIGHT_PROGRAM);
            }
            // we increment elapsedInModeCounter twice a sec, so we divide by 2
            else if (elapsedInModeCounter / 10 >= config.timeDelay) {
                setMode(MODE_FLY);
            }
            // NOTE this has to be in sync with BLINK_FAST blinking, hence the high rate
            else if (elapsedInMode(100)) {
                drawRemainingTime(config.timeDelay - (currentTime - currentModeStarted) / 1000);
            }
            break;
        // @todo soft start?
        case MODE_SOFT_START:
            break;
        case MODE_FLY:
            if (NOT_IMPLEMENTED) {
                notImplemented();
                return;
            }
            else if (ANY_BUTTON_PUSHED) {
                setMode(MODE_ERR);
                return;
            }
            // this makes a funny fastflash-pause-fastflash-pause pattern
            ledOn();
            if (elapsedInMode(100)) {

                // update throttle - currently only fixed throttle
                i = config.throttle;
                throttlePcnt(i);

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
            }
            break;
        // draw after info
        case MODE_AFTER:
        // some error happened - cut threshold reached or button pressed. Draw error and also after info
        case MODE_ERR:
            if (elapsedInMode(200)) {
                bool drawScreen = true;
                if (btnAPushed()) {
                    currentScreen+= 4;
                }
                else if (btnBPushed()) {
                    currentScreen++;
                }
                else if (elapsedInModeCounter % 10 == 0) {
                    currentScreen++;
                }
                else {
                    drawScreen = false;
                }
                if (currentScreen > 4) {
                    currentScreen-= 5;
                }
                if (drawScreen) {
                    drawAfterScreen(currentScreen + (currentMode == MODE_AFTER ? 1 : 0));
                }
            }
            break;
    }

}

#endif
#endif

// some crazy flashing
void notImplemented() {
    throttleOff();
    if (elapsedInMode(50)) {
        drawNotImplemented();
    }
}

void setMode(int newMode) {
    currentMode = newMode;
    currentModeStarted = currentTime;
    elapsedInModeCounter = 0;
    switch (newMode) {
        case MODE_TEST_SPIN:
            resetMetrics();
        case MODE_DELAY:
            if (!NOT_IMPLEMENTED) {
                drawArming();
                armThrottle();
            }
        // commenting or not the following makes no difference in memory, probably it's optimized during compile
//            clearScreen();
//        break;
        case MODE_PREFLIGHT_PROGRAM:
        case MODE_DELAY_LOCK:
        case MODE_TEST:
            clearScreen();
            break;
        case MODE_TEST_SAVED:
            drawSaved();
        break;
        case MODE_AFTER:
            drawAfterScreen(0);
        break;
        case MODE_ERR:
            drawAfterScreen(0);
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
    else if (elapsedInModeCounter > 11) {
        setMode(nextMode);
    }
    else if (elapsedInMode(DELAY_COUNTDOWN)) {
        drawFlyConfirmation(true);
        drawWaitDot(elapsedInModeCounter);
    }
    blinkLed(BLINK_FAST);
}