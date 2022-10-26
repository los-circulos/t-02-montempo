#include <Arduino.h>
#include "config.h"
#include "hardware.h"
#include "main.h"
#include "mytime.h"
#include <screen.h>

uint8_t currentMode = MODE_WELCOME_LOCK;
//uint8_t currentMode = MODE_CONFIG;
//uint8_t currentMode = MODE_DELAY;
unsigned long currentModeStarted = 0;
// elapsed incremental cycles since last mode change. !! use only through elapsedInMode().
unsigned long elapsedInModeCounter = 0;

void setMode(int newMode);
bool elapsedInMode(unsigned int);

void setup() {

//    Serial.begin(9600);

    initConfig();
    initScreen();

    readConfig();

    drawWelcome();

}

void loop() {

    setCurrentTime();
    int i;

    switch (currentMode) {
        case MODE_WELCOME_LOCK:
            // @todo during welcome lock I should show previous flight values
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

                readConfig();
                drawScreen(config);

                if (btnBDisabled() || btnBPushed()) {
                    if (btnADisabled() || btnAPushed()) {
                        setMode(MODE_CONFIG_COUNTDOWN);
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

            break;
        case MODE_CONFIG_COUNTDOWN:
            if ((!btnADisabled() && !btnAPushed()) || (!btnBDisabled() && !btnBPushed())) {
                setMode(MODE_CONFIG);
                drawFlyConfirmation(false);
            }
            else if (elapsedInModeCounter > 6) {
                setMode(MODE_DELAY_LOCK);
            }
            else if (elapsedInMode(DELAY_COUNTDOWN)) {
                drawFlyConfirmation(true);
                drawWaitDot(elapsedInModeCounter);
            }
            blinkLed(BLINK_FAST);
            break;
        case MODE_DELAY_LOCK:
            if (!ANY_BUTTON_PRESSED) {
                setMode(MODE_DELAY);
                eraseLogoLock();
            }
            else if (elapsedInMode(100)) {
                drawLogoLock();
            }

            break;
        case MODE_DELAY:
            if (ANY_BUTTON_PRESSED) {
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
            if (config.holdRPM) {

            }
            else if (config.holdCurrent) {

            }
            else {
                throttle.writeMicroseconds(config.throttle);
            }
            break;
        case MODE_AFTER:
            break;
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
            clearScreen();
        break;
        case MODE_TEST:
        case MODE_DELAY:
            throttle.attach(PIN_THROTTLE);
            throttle.writeMicroseconds(THROTTLE_MICROS_MIN);
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
