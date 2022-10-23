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

int currentMode = MODE_LOGO_LOCK;

void setup() {

    initConfig();
    initScreen();

    readConfig();

    // display logo, version
    drawWelcome();
    trapWhileAnyButtonPressed();

    clearScreen();
}

void loop() {

    readConfig();
    drawScreen(config);
    delay(200);

// write your code here
//    u8x8.clear();
////  u8x8.setFont(u8x8_font_chroma48medium8_r);
//    u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
//    u8x8.drawString(0, 0, "0123456789");
//    readConfig();
//    readConfig();
//    readConfig();
//    readConfig();
//    readConfig();
//    readConfig();
//    readConfig();

}

void trapWhileAnyButtonPressed() {

    int i, j;

    // to continue from welcome screen, we require that neither button is pressed
    do {
        // lock on logo screen if any of the 2 buttons are pushed accidentally
        while (BTN_A_PUSHED || BTN_B_PUSHED) {
            drawLogoLock();
        }
        // when neither is pushed, draw 6 dots in the bottom and if any button is pressed, start over
        for (i=4; i<11; i++) {
            u8x8.drawString(i, 3, ".");
            // since I cannot use an interrupt on button pins, I'll do this lame loop to watch for button pushes while dots are being drawn
            // not doing the loop saves >100bytes of memory but then intermittent presses might pass uncaught
            for (j=0; j<20; j++) {
                if (BTN_A_PUSHED || BTN_B_PUSHED) {
                    i = 20;
                }
                delay(10);
            }
        }
    } while (i >= 20);
}
