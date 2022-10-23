#include <Arduino.h>
#include "config.h"
#include "hardware.h"
#include "main.h"
#include <U8g2lib.h>
#include <Wire.h>
#include <screen.h>
//#include <U8x8lib.h>

//public: U8X8_SSD1306_128X32_UNIVISION_HW_I2C(uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8X8() {
U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED

// for u8g2 this works
//U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED

bool asd = true;

void setup() {
// write your initialization code here
    readConfig();
    u8x8.begin();
    u8x8.setPowerSave(0);

    // @todo display logo, version

    // show current settings
    u8x8.clear();
    u8x8.setFont(FONT_S);
    u8x8.drawString(0, 3, "F2B");
    u8x8.drawString(11, 3, "V1.0");
    u8x8.setFont(FONT_XL);
    u8x8.drawString(0, 0, "MONTEMPO");
    delay(2000);

    u8x8.clear();

    if (config.testMode) {
        u8x8.drawString(0, 0, "TEST");
    }
    else {
        u8x8.drawString(0, 0, "FLY?");
    }
    u8x8.setFont(FONT_S);
//    if (config.holdRPM || true) {
    if (config.holdRPM) {
        u8x8.drawString(0, 3, "RPM 8.5");
    }
//    else if (config.holdCurrent || true) {
    else if (config.holdCurrent) {
        u8x8.drawString(0, 3, "NOTYET:(");
    }
    else {
        u8x8.drawString(0, 3, "THR  82%");
    }

    u8x8.setFont(FONT_L);
    u8x8.drawString(10,0, "12.4 V");
    u8x8.drawString(11, 2, "10:10");

}

void loop() {

//    readConfig();
//    delay(100);

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
