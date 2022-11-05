#include <Arduino.h>
#include "config.h"
#include "hardware.h"
#include "main.h"
#include "mytime.h"
#include "saved.h"
#include <screen.h>

uint8_t currentMode = MODE_WELCOME_LOCK;
unsigned long currentModeStarted = 0;
// elapsed incremental cycles since last mode change. !! use only through elapsedInMode().
unsigned long elapsedInModeCounter = 0;

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
    initScreen();

    readConfig();

    drawWelcome();

    // this will leave the ESC initialized but not armed
    throttle.attach(PIN_THROTTLE);
    throttle.writeMicroseconds(0);

    delay(2000);
    armThrottle();
    delay(2000);

    setMode(MODE_WELCOME_LOCK);

}

//#define AFTER_SCREEN_DEBUG
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
char buffe[20];
float ji = 0;
unsigned long almaja = 0;
void loop() {

    setCurrentTime();
    unsigned int i;

    throttlePcnt(20);
//    throttlePcnt(50);

#define  BASEV 5.0

//    if (elapsedInMode(100)) {
    if (elapsedInMode(20)) {
//    if (elapsedInMode(10)) {

        i = analogRead(PIN_CURRENT);
        almaja+= i;

//        float jj = (BASEV * i / 512 - BASEV) / 0.132;
//        float jj = (BASEV * i / 1024 - BASEV/2) / 0.066;
//        float jj = (BASEV * i / 1024 - BASEV/2) * 15.152;
        float jj = (BASEV * i / 512 - BASEV) * 7.576;
        if (jj < 0) {
            jj = -jj;
        }

        ji+= jj;

//        if (elapsedInModeCounter % 10 == 0) {
        if (elapsedInModeCounter % 50 == 0) {
//        if (elapsedInModeCounter % 100 == 0) {
            u8x8.setFont(FONT_L);
//            sprintf(buffe, "%3d %10d ", i, ji);
            sprintf(buffe, "%3d %10lu", i, almaja);
            u8x8.drawString(0, 0, buffe);
//            sprintf(buffe, "%3d %10d", (int)(ji/10), (int)jj);
            sprintf(buffe, "%6d %6d", (int)(almaja/50), (int)(ji/50));
//            sprintf(buffe, "%6d %6d", (int)(almaja/100), (int)(ji/100));
            u8x8.drawString(0, 2, buffe);
            ji = 0;
            almaja = 0;
        }

    }

    return;

    i = analogRead(PIN_VOLT) / INPUT_DIV_VOLT;
    sprintf(buffe, " %3d    ", i);
    u8x8.setFont(FONT_L);
    u8x8.drawString(0, 0, buffe);

    float jj = (BASEV * analogRead(PIN_CURRENT) / 512 - BASEV) / 0.132;
    if (jj < 0) {
        jj = -jj;
    }

//    sprintf(buffe, "  %6d  %6d", ii, jj);
//    sprintf(buffe, "%4d %4d %4d", (int)(ii*100), (int)(jj*100), analogRead(PIN_CURRENT));
    sprintf(buffe, "%4d %4d %4d", (int)(jj*100), (int)(jj*100), analogRead(PIN_CURRENT));
    u8x8.drawString(0, 2, buffe);

    delay(100);
    return;

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
                // for testing only
//                if ((testMode != TESTMODE_SPIN)) {
                readTestConfig();
                if ((testMode != TESTMODE_SPIN) || !ANY_BUTTON_PUSHED) {
                    setMode(MODE_WELCOME_LOCK);
                    throttlePcnt(0);
                }
                else {
//                    readTele();
                    drawRunScreen();
                    throttlePcnt(testValue);
                }
            }
            break;
        case MODE_PREFLIGHT_PROGRAM:
            if (elapsedInMode(200)) {
                readConfig();
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