#include <Arduino.h>
#include "config.h"
#include "hardware.h"
#include "main.h"
#include <U8x8lib.h>

U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED

bool asd = true;

void setup() {
// write your initialization code here
    readConfig();
    u8x8.begin();
    u8x8.setPowerSave(0);

    // @todo display logo, version

    // show current settings
    u8x8.clear();
    u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
    u8x8.drawString(0, 0, "0123456789");
    u8x8.drawString(0, 1, "0123456789");

}

void loop() {
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
