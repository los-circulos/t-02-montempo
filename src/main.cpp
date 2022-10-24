#include <Arduino.h>
#include "config.h"
#include "hardware.h"
#include "main.h"
#include "time.h"
//#include <U8g2lib.h>
#include <Wire.h>
#include <screen.h>
#include <U8x8lib.h>

////public: U8X8_SSD1306_128X32_UNIVISION_HW_I2C(uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8X8() {
//U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED

// for u8g2 this works
//U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED

int currentMode = MODE_WELCOME_LOCK;
unsigned long currentModeStarted = 0;
// elapsed incremental cycles since last mode change. !! use only through elapsedInMode().
unsigned long elapsedInModeCounter = 0;

unsigned long m;

void setMode(int newMode);

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
//            if (!btnAnyButtonPressed()) {
            if (!ANY_BUTTON_PRESSED) {
                setMode(MODE_WELCOME_COUNTDOWN);
                eraseLogoLock();
            }
            else {
                drawLogoLock();
            }
            break;
        case MODE_WELCOME_COUNTDOWN:
//            if (btnAnyButtonPressed()) {
            if (ANY_BUTTON_PRESSED) {
                setMode(MODE_WELCOME_LOCK);
            }
            else if (elapsedInModeCounter > 6) {
                setMode(MODE_CONFIG);
            }
            else if (elapsedInMode(250)) {
                u8x8.drawString(elapsedInModeCounter + 4, 3, ".");
            }
            break;
        case MODE_TEST:
            break;
        case MODE_CONFIG:
            if (btnBDisabled() || btnBPushed()) {
                u8x8.drawString(3, 0, "?");
            }
            else {
                u8x8.drawString(3, 0, " ");
            }
            if (elapsedInMode(200)) {
                // @todo time this so we can see if it interferes with servo signal at all
                readConfig();
                drawScreen(config);
            }
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
    }
}

/**
 * A tricky stepper for "incremental ticks"
 * eg. a waiting bar from . to ...... is implemented in MODE_WELCOME_COUNTDOWN. It uses the return value
 *  and if 250 millis have elapsed since drawing the last dot, it will draw another dot.
 * @param elapsedMillis
 */
bool elapsedInMode(unsigned int elapsedMillis) {
    unsigned int elapsedCnt = (m - currentModeStarted) / elapsedMillis;
    if (elapsedCnt > elapsedInModeCounter) {
        elapsedInModeCounter = elapsedCnt;
        return true;
    }
    return false;
}
