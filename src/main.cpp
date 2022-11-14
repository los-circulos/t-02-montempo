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
//#define AMP_DEBUG

unsigned char currentMode = MODE_WELCOME_LOCK;
unsigned long currentModeStarted = 0;
// elapsed incremental cycles since last mode change. !! use only through elapsedInMode().
unsigned long elapsedInModeCounter = 0;
unsigned char alarm = 0;
unsigned long alarmStarted = 0;

#ifdef RPM_DEBUG
int xPcnt = 5;
volatile unsigned int rpmCnt = 0;
void rpmISR() {
    rpmCnt++;
}
#endif

#define MODE_NOT_IMPLEMENTED (saved.holdMode != HOLD_MODE_HOLD_THROTTLE)
void notImplemented();
void setMode(unsigned char newMode);
void endMode(unsigned char result);
bool elapsedInMode(unsigned int);
void confirmation();
void countDown(char nextMode);
void setAlarm(unsigned char resultCode, bool conditionMet);

void setup() {

//    Serial.begin(9600);

    initSaved();
    initConfig();
    initHardware();
    initScreen();

    readConfigInput();

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
#ifdef AMP_DEBUG
char buf[20];
void loop() {
//    if (elapsedInMode(200)) {

        readMetrics();
        sumMetrics();

        sprintf(buf, "%6d", analogRead(PIN_CURRENT));
        u8x8.drawString(0, 0, buf);
        sprintf(buf, "%5d %8d", metrics.amps/5, METRICS_AVG_AMPS);
        u8x8.drawString(0, 2, buf);
//    }
    delay(200);
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
#ifdef DEVMODE
            else if (elapsedInModeCounter > 2) {
#else
            else if (elapsedInModeCounter > 8) {
#endif
                if (config.savedInputMode) {
                    setMode(MODE_SAVED_INPUT);
                }
                else {
                    setMode(MODE_PREFLIGHT_PROGRAM);
                }
            }
            else if (elapsedInMode(DELAY_COUNTDOWN)) {
                drawWaitDot(elapsedInModeCounter);
            }
        break;
        case MODE_SAVED_INPUT:
            // @todo deny test/setup if both buttons are disabled
            if (elapsedInMode(200)) {
                readSavedInput();
                drawSavedInputScreen();
                confirmation();
            }
        break;
        case MODE_SAVED_INPUT_COUNTDOWN:
            countDown(MODE_SAVED_INPUT_SAVE);
        break;
        case MODE_SAVED_INPUT_SAVE:
            if (savedInputMode == SAVED_INPUT_MODE_SPIN) {
                setMode(MODE_TEST_SPIN);
                return;
            }
            else if (savedInputMode == SAVED_INPUT_MODE_SMART) {
                saved.smartEndThrottle = savedInputValue;
            }
            else if (savedInputMode == SAVED_INPUT_MODE_T1_CUT) {
                saved.t1Cut = savedInputValue;
            }
            else if (savedInputMode == SAVED_INPUT_MODE_T2_CUT) {
                saved.t2Cut = savedInputValue;
            }
            else if (savedInputMode == SAVED_INPUT_MODE_VOLT_CUT) {
                saved.voltCut = savedInputValue;
            }
            else if (savedInputMode == SAVED_INPUT_MODE_CURRENT_CUT) {
                saved.currentCut = savedInputValue;
            }
            else if (savedInputMode == SAVED_INPUT_MODE_MODE) {
                saved.holdMode = savedInputValue;
            }
            else if (savedInputMode == SAVED_INPUT_MODE_POLES) {
                saved.poles = savedInputValue;
            }
            saveSaved();
            setMode(MODE_SAVED_INPUT_SAVED);
        break;
        case MODE_SAVED_INPUT_SAVED:
            if (elapsedInMode(100)) {
                if (ANY_BUTTON_PUSHED) {
                    drawLogoLock();
                    ledOn();
                    currentModeStarted = currentTime;
                    elapsedInModeCounter = 0;
                }
                else if (elapsedInModeCounter > 8) {
                    setMode(MODE_SAVED_INPUT);
//                    return;
                }
                else {
                    drawWaitDot(elapsedInModeCounter);
                }
            }
        break;
        case MODE_TEST_SPIN:
            if (elapsedInMode(200)) {
                readSavedInput();
                readAndSumMetrics();
                if ((savedInputMode != SAVED_INPUT_MODE_SPIN) || !ANY_BUTTON_PUSHED) {
                    setMode(MODE_WELCOME_LOCK);
                    throttlePcnt(0);
                }
                else {
                    drawTestSpinScreen();
                    throttlePcnt(savedInputValue);
                }
            }
        break;
        case MODE_PREFLIGHT_PROGRAM:
            if (elapsedInMode(200)) {
                readConfigInput();
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
            i = config.timeDelay - (currentTime - currentModeStarted) / 1000;
            if (MODE_NOT_IMPLEMENTED) {
                notImplemented();
            }
            else if (ANY_BUTTON_PUSHED) {
                setMode(MODE_PREFLIGHT_PROGRAM);
            }
            else if (i == 0) {
                drawRemainingTime(i);
                ledOn();
                delay(1000);
                setMode(MODE_FLY);
            }
            // NOTE this has to be in sync with BLINK_FAST blinking, hence the otherwise unnecessarily high rate
            else if (elapsedInMode(100)) {
                drawRemainingTime(i);
                blinkLed(i < 5 ? BLINK_FAST : BLINK_SLOW);
            }
        break;
        // @todo soft start?
        case MODE_SOFT_START:
            break;
        case MODE_FLY:
            if (MODE_NOT_IMPLEMENTED) {
                notImplemented();
                return;
            }
            else if (ANY_BUTTON_PUSHED) {
                endMode(RESULT_ERR_BTN);
                return;
            }

            // loop every 0.1sec
            if (elapsedInMode(100)) {

                readAndSumMetrics();

                // SET ALARMS
                setAlarm(
                    RESULT_ERR_VCUT,
                    (metrics.volts > 0) && (config.cellCount > 0) && (config.cellCount * (30+saved.voltCut) > metrics.volts)
                );
//                // @todo we might not need this???
                setAlarm(RESULT_ERR_V_OVER, metrics.volts > config.cellCount * 42);
                setAlarm(RESULT_ERR_ACUT, metrics.amps / 5 > saved.currentCut);
                setAlarm(RESULT_ERR_RPM_OVER, metrics.rpm > RPM_MAX);

                // 606

                // UPDATE THROTTLE
                // update holdThrottle - currently only fixed holdThrottle
                if (metricsSum.holdMode == HOLD_MODE_HOLD_THROTTLE) {
                    throttlePcnt(config.holdThrottle);
                }

                // elapsed time
                i = (currentTime - currentModeStarted) / 1000;

                // timed flight OR
                if (config.timeFly > 0) {
                    if (i >= config.timeFly) {
                        endMode(RESULT_OK_TIME);
                        return;
                    }
                    // remaining time is flight time minus soft start time (elapsed already) minus elapsed time
                    i = config.timeFly - config.softStartTime - i;
                }
                // until cut flight
                else {
                    // @todo limit flight to 9:59
                    // @todo check here if voltage reading is meaningful and error if not
                    // when using soft start and incremental time, add it to elapsed time (twice to increment previous deduction)
                    i+= 2*config.softStartTime;
                }

                // blink led fast if less than 5 seconds remain AND ALSO in every first half of a second
                #define LESS_THAN_5_SECONDS_REMAIN ((i < 5) && config.timeFly > 0)
                // @todo would be nice to blink fast too before a cutdown happens
                if (LESS_THAN_5_SECONDS_REMAIN || ((elapsedInModeCounter % 10) < 5)) {
                    blinkLed(BLINK_FAST);
                }
                else {
                    ledOff();
                }

                // update screen every 0.5sec
                if ((elapsedInModeCounter % 5) == 0) {
                    drawRunScreen(i);
                }
            }
            break;
        // draw after info
        case MODE_AFTER:
            if (elapsedInMode(200)) {
                bool drawScreen = true;
                if (btnAPushed()) {
                    currentScreen+= 4;
                }
                else if (btnBPushed()) {
                    currentScreen++;
                }
                else if (config.rotateScreens && (elapsedInModeCounter % 25 == 0)) {
                    currentScreen++;
                }
                else {
                    drawScreen = false;
                }
                if (currentScreen > 4) {
                    currentScreen-= 5;
                }
                if (drawScreen) {
                    drawAfterScreen(currentScreen);
                    // delay a bit, helps if button is still pressed
                    delay(500);
                }
            }
            break;
    }

}

#endif
#endif
#endif

// some crazy flashing
void notImplemented() {
    throttleOff();
    if (elapsedInMode(50)) {
        drawNotImplemented();
    }
}

void setMode(unsigned char newMode) {
    currentMode = newMode;
    currentModeStarted = currentTime;
    elapsedInModeCounter = 0;
    switch (newMode) {
        case MODE_TEST_SPIN:
            resetMetrics();
        case MODE_DELAY:
            if (!MODE_NOT_IMPLEMENTED) {
                drawArming();
                armThrottle();
            }
        // commenting or not the following makes no difference in memory, probably it's optimized during compile
//            clearScreen();
//            break;
        case MODE_PREFLIGHT_PROGRAM:
        case MODE_DELAY_LOCK:
        case MODE_SAVED_INPUT:
            clearScreen();
        break;
        case MODE_SAVED_INPUT_SAVED:
            drawSaved();
        break;
        case MODE_FLY:
            resetMetrics();
        break;
    }
    ledOff();
}

void endMode(unsigned char result) {
    throttleOff();
    // masq ERR_VCUT for flights without time limit
    metricsSum.result = ((result == RESULT_ERR_VCUT) && (config.timeFly == 0)) ? RESULT_OK_V : result;
    // display "WELLDONE" or "OOPS" as long as initial delay, then...
    drawAfterScreen(0);
    // @TODO save flight metrics here (should set flight number!)
    delay(config.timeDelay*1000);
    currentScreen = 1;
    // ... then draw main after screen
    drawAfterScreen(currentScreen);
    setMode(MODE_AFTER);
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
#ifdef DEVMODE
    else if (elapsedInModeCounter > 3) {
#else
    else if (elapsedInModeCounter > 11) {
#endif
        setMode(nextMode);
    }
    else if (elapsedInMode(DELAY_COUNTDOWN)) {
        drawFlyConfirmation(true);
        drawWaitDot(elapsedInModeCounter);
    }
    blinkLed(BLINK_FAST);
}

void setAlarm(unsigned char resultCode, bool conditionMet) {
    bool alarmElapsed = (currentTime - alarmStarted) > 1000;
    if (conditionMet) {
        if ((alarm == resultCode) && alarmElapsed) {
            endMode(alarm);
        }
        else if (alarm == 0) {
            alarm = resultCode;
            alarmStarted = 0;
        }
    }
    else if (alarm == resultCode) {
        alarm = 0;
    }
}
