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

#define MODE_NOT_IMPLEMENTED (((saved.holdMode != HOLD_MODE_HOLD_THROTTLE) && (saved.holdMode != HOLD_MODE_SMART_THROTTLE)) || \
    ((saved.holdMode == HOLD_MODE_SMART_THROTTLE) && (config.timeFly == 0)))
void notImplemented();
void setMode(unsigned char newMode);
void endMode(unsigned char result);
bool elapsedInMode(unsigned int);
void confirmation();
void countDown(char nextMode);
void flightAlarms();
void setAlarm(unsigned char resultCode, bool conditionMet);

void setup() {

//    Serial.begin(9600);

    initScreen();
    initSaved();
    initHardware();
    initConfig();

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

    if (elapsedInMode(E_EVERY_SEC)) {
        u8x8.setFont(FONT_L);
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

    unsigned int flyElapsed = (currentTime - currentModeStarted) / 1000;

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
                    setMode(MODE_PREFLIGHT_CONFIG);
                }
            }
            else if (elapsedInMode(DELAY_COUNTDOWN)) {
                drawWaitDot(elapsedInModeCounter);
            }
        break;
        case MODE_SAVED_INPUT:
            // @todo deny test/setup if both buttons are disabled
            if (elapsedInMode(E_5_PER_SEC)) {
                readSavedSetup();
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
            else if (savedInputMode == SAVED_INPUT_MODE_T1_CUT) {
                saved.t1Cut = savedInputValue;
            }
            else if (savedInputMode == SAVED_INPUT_MODE_T2_CUT) {
                saved.t2Cut = savedInputValue;
            }
            else if (savedInputMode == SAVED_INPUT_MODE_MODE) {
                saved.holdMode = savedInputValue;
            }
            else if (savedInputMode == SAVED_INPUT_MODE_VOLT_CUT) {
                saved.voltCut = savedInputValue;
            }
            else if (savedInputMode == SAVED_INPUT_MODE_POLES) {
                saved.poles = savedInputValue;
            }
            else if (savedInputMode == SAVED_INPUT_MODE_END_THROTTLE) {
                saved.endValue = savedInputValue;
            }
            else if (savedInputMode == SAVED_INPUT_MODE_GOVI) {
                saved.govi = !!savedInputValue;
            }
            else if (savedInputMode == SAVED_INPUT_MODE_ARM) {
                saved.arm = !!savedInputValue;
            }
            else if (savedInputMode == SAVED_INPUT_MODE_CAL) {
                saved.calibrate = !!savedInputValue;
            }
            else if (savedInputMode == SAVED_INPUT_MODE_SOFT_TIME) {
                saved.softTime = savedInputValue;
            }
            else if (savedInputMode == SAVED_INPUT_MODE_COUNTDOWN) {
                saved.countdown = savedInputValue;
            }
            else if (savedInputMode == SAVED_INPUT_MODE_CLEAR_LOGS) {
                notImplemented();
                return;
            }
            saveSavedSetup();
            setMode(MODE_SAVED_INPUT_SAVED);
#ifdef PIN_CURRENT
            else if (savedInputMode == SAVED_INPUT_MODE_CURRENT_CUT_OBS) {
                saved.currentCut = savedInputValue;
            }
#endif
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
                }
                else {
                    drawWaitDot(elapsedInModeCounter);
                }
            }
        break;
        case MODE_TEST_SPIN:
            if (elapsedInMode(E_5_PER_SEC)) {
                readSavedSetup();
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
        case MODE_PREFLIGHT_CONFIG:
            if (elapsedInMode(E_5_PER_SEC)) {
                readConfigInput();
                readMetrics();
                drawPreflight(config);
                // for some reason I don't see config.preflightError properly here so this if() never succeeds
                if (config.preflightError) {
                    drawNoFly();
                    delay(500);
                }
                else {
                    confirmation();
                }
            }
        break;
        case MODE_PREFLIGHT_CONFIG_COUNTDOWN:
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
#ifdef DEVMODE
            i = 3 - (currentTime - currentModeStarted) / 1000;
#else
            i = saved.countdown - (currentTime - currentModeStarted) / 1000;
#endif
            if (MODE_NOT_IMPLEMENTED) {
                notImplemented();
            }
            else if (ANY_BUTTON_PUSHED) {
                setMode(MODE_WELCOME_LOCK);
            }
            else if (i == 0) {
                // without this, it looks ugly and just 8 bytes
                drawRemainingTime(i);
                ledOn();
                delay(1000);
                // @TODO enter soft start mode only if config.softStartTime > 0
//                setMode(MODE_FLY);
                setMode(MODE_SOFT_START);
            }
            // NOTE this has to be in sync with BLINK_FAST blinking, hence the otherwise unnecessarily high rate
            else if (elapsedInMode(100)) {
                drawRemainingTime(i);
                blinkLed(i < 5 ? BLINK_FAST : BLINK_SLOW);
            }
        break;
        case MODE_SOFT_START:
            if (MODE_NOT_IMPLEMENTED) {
                notImplemented();
            }
            else if (ANY_BUTTON_PUSHED) {
                endMode(RESULT_ERR_BTN);
            }
            else if (elapsedInMode(100)) {

                flightAlarms();

                if (flyElapsed >= saved.softTime) {
                    setMode(MODE_FLY);
                }
                // 5/target = elapsed/x
                i = flyElapsed * config.holdThrottle / saved.softTime;
                throttlePcnt(i);
                blinkLed(BLINK_NORMAL);

                if ((elapsedInModeCounter % 2) == 0) {
                    drawRunScreen(flyElapsed - saved.softTime);
                }

            }
            break;
        case MODE_FLY:
            if (MODE_NOT_IMPLEMENTED) {
                notImplemented();
            }
            else if (ANY_BUTTON_PUSHED) {
                endMode(RESULT_ERR_BTN);
            }
            // loop every 0.1sec
            else if (elapsedInMode(100)) {

                readAndSumMetrics();

                flightAlarms();
                // if an alarm has shut the system down, return - so run screen is not displayed again
                if (currentMode != MODE_FLY) {
                    return;
                }

                // elapsed time already in flyelapsed

                // UPDATE THROTTLE
                // update holdThrottle - currently only fixed holdThrottle
                if (metricsSum.holdMode == HOLD_MODE_HOLD_THROTTLE) {
                    throttlePcnt(config.holdThrottle);
                }
                else if (metricsSum.holdMode == HOLD_MODE_SMART_THROTTLE) {
                    // timeLimit / elapsed = (thr1 - thr0) / (thrX - thr0)
                    // elapsed / timeLimit = (thrX - thr0) / (thr1 - thr0)
                    // thrX = thr0 + (thr1 - thr0) * elapsed / timeLimit
                    i = (unsigned long)(saved.endValue - config.holdThrottle) * flyElapsed / config.timeFly + config.holdThrottle;
                    throttlePcnt(i);
                }

                // timed flight OR
                if (config.timeFly > 0) {
                    if (flyElapsed >= config.timeFly) {
                        endMode(RESULT_OK_TIME);
                        return;
                    }
                    // remaining time is flight time minus soft start time (elapsed already) minus elapsed time
                    i = config.timeFly - flyElapsed;
                }
                // until cut flight
                else {
                    // @todo limit flight to 9:59
                    // @todo check here if voltage reading is meaningful and error if not
                    // when using soft start and incremental time, add it to elapsed time (twice to increment previous deduction)
                    i = flyElapsed;
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
                    clearScreen();
                    drawRunScreen(i);
                }
            }
            break;
        // draw after info
        case MODE_AFTER:
            if (elapsedInMode(E_5_PER_SEC)) {
                bool drawScreen = true;
                if (btnAPushed()) {
                    currentScreen+= AFTER_SCREENS_COUNT-1;
                }
                else if (ANY_BUTTON_PUSHED) {
                    currentScreen++;
                }
//                else if (config.rotateScreens && (elapsedInModeCounter % (config.screenRotateTime * 5) == 0)) {
                else if ((elapsedInModeCounter % (config.screenRotateTime * 5)) == 0) {
                    currentScreen++;
                }
                else {
                    drawScreen = false;
                }

                if (currentScreen >= AFTER_SCREENS_COUNT) {
                    currentScreen-= AFTER_SCREENS_COUNT;
                }

                if (drawScreen) {
                    drawAfterScreen(currentScreen);
                    // delay a bit, helps if button is still pressed
                    delay(500);
                }
            }
#ifdef DEVMODE
            confirmation();
            break;
            case MODE_AFTER + 1:
                countDown(MODE_WELCOME_LOCK);
#endif
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

void setMode(unsigned char newMode) {
    switch (newMode) {
        case MODE_TEST_SPIN:
            resetMetrics();
        case MODE_DELAY:
            if (!MODE_NOT_IMPLEMENTED) {
                armThrottle();
            }
        // commenting or not the following makes no difference in memory, probably it's optimized during compile
//            clearScreen();
//            break;
        case MODE_PREFLIGHT_CONFIG:
        case MODE_DELAY_LOCK:
        case MODE_SAVED_INPUT:
            clearScreen();
        break;
        case MODE_SAVED_INPUT_SAVED:
            drawSaved();
        break;
        case MODE_SOFT_START:
            resetMetrics();
        break;
        case MODE_FLY:
            if (currentMode != MODE_SOFT_START) {
                resetMetrics();
            }
            clearScreen();
        break;
    }
    currentMode = newMode;
    currentModeStarted = currentTime;
    elapsedInModeCounter = 0;
    ledOff();
}

void endMode(unsigned char result) {
    throttleOff();
    // masq ERR_VCUT for flights without time limit
    metricsSum.result = ((result == RESULT_ERR_VCUT) && (config.timeFly == 0)) ? RESULT_OK_V : result;
    // display "WELLDONE" or "OOPS" as long as initial delay, then...
    drawAfterScreen(0);
    // @TODO save flight metrics here (should set flight number!)
//    delay(config.timeDelay*1000);
    delay(2000);
    saveMetrics();
    currentScreen = 1;
//    clearScreen();
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
    if (ANY_BUTTON_PUSHED) {
        if (ALL_BUTTONS_PUSHED) {
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
    if (!ALL_BUTTONS_PUSHED) {
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

void flightAlarms() {

    // SET ALARMS
    setAlarm(
        RESULT_ERR_VCUT,
        (metrics.volts > 0) && (config.cellCount > 0) && (config.cellCount * (30+saved.voltCut) > metrics.volts)
    );
//    // @todo we might not need this???
    setAlarm(
        RESULT_ERR_V_OVER,
        !VOLTS_DISABLED && (metrics.volts > config.cellCount * 42)
    );
#ifdef PIN_CURRENT
    setAlarm(
        RESULT_ERR_ACUT,
        !CURRENT_DISABLED && (metrics.amps / 5 > saved.currentCut)
    );
#endif
    setAlarm(
        RESULT_ERR_RPM_OVER,
        (metrics.rpm > 0) && (metrics.rpm > RPM_MAX)
    );

}
void setAlarm(unsigned char resultCode, bool conditionMet) {
    bool alarmElapsed = (currentTime - alarmStarted) > ALARM_TIMEOUT;
    if (conditionMet) {
        if ((alarm == resultCode) && alarmElapsed) {
            endMode(alarm);
        }
        else if (alarm == 0) {
            alarm = resultCode;
            alarmStarted = currentTime;
        }
        return;
    }
    else if (alarm == resultCode) {
        alarm = 0;
    }
}
