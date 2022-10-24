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

void setup() {

    initConfig();
    initScreen();

    readConfig();

    clearScreen();
    drawWelcome();
}

void loop() {

    unsigned long m = millis();
    int i;

    switch (currentMode) {
        case MODE_WELCOME_LOCK:
            // display logo, version
            if (!BTN_ANY_BUTTON_PRESSED) {
                currentMode = MODE_WELCOME_COUNTDOWN;
                currentModeStarted = m;
            }
            else {
                drawLogoLock();
            }
            break;
        case MODE_WELCOME_COUNTDOWN:
            if (BTN_ANY_BUTTON_PRESSED) {
                currentMode = MODE_WELCOME_LOCK;
            }
            else {
                i = (m - currentModeStarted) / 200;
                u8x8.drawString(i+4, 3, ".");
                if (i>6) {
                    currentMode = MODE_CONFIG;
                }
            }
            break;
        case MODE_TEST:
            break;
        case MODE_CONFIG:
            readConfig();
            drawScreen(config);
            delay(200);
            break;
        case MODE_DELAY:
            break;
        case MODE_FLY:
            break;
        case MODE_AFTER:
            break;
    }

}
