#ifndef MONTEMPO_SCREEN_H
#define MONTEMPO_SCREEN_H

////////////////////////////////////////////////////////////////////////////////
// screen hardware specific START
////////////////////////////////////////////////////////////////////////////////
#ifdef SCREEN_32X4

#include <U8x8lib.h>

//public: U8X8_SSD1306_128X32_UNIVISION_HW_I2C(uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8X8() {
//extern U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED
extern U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8;

#define FONT_S u8x8_font_amstrad_cpc_extended_u
//#define FONT_S u8x8_font_amstrad_cpc_extended_r
#define FONT_L u8x8_font_8x13B_1x2_f
#define FONT_XL u8x8_font_profont29_2x3_r

#endif
////////////////////////////////////////////////////////////////////////////////
// screen hardware specific END
////////////////////////////////////////////////////////////////////////////////

#define SCREEN_PRE 1
#define SCREEN_TEST 100

extern int currentScreen;

void initScreen();
void clearScreen();

void drawArming();
void drawFlyConfirmation(bool show);
void drawLogoLock();
void eraseLogoLock();
void drawNotImplemented();
void drawRemainingTime(unsigned int secsRemain);
void drawSaved();
void drawWaitDot(uint8_t x);

void drawWelcome();
void drawPreflight(configT config);
void drawTestScreen();
void drawRunScreen();
void drawAfterScreen(unsigned char which);

#endif //MONTEMPO_SCREEN_H
