#ifndef MONTEMPO_HARDWARE_H
#define MONTEMPO_HARDWARE_H

#include <Arduino.h>
#include "Servo.h"

///////////////////////////////////////// LEDS          /////////////////////////////////////
#define LED1 LED_BUILTIN

void ledOn();
void ledOff();

///////////////////////////////////////// BUTTONS       /////////////////////////////////////
#define BTN_A PIN_A7
#define BTN_B PIN_A6

bool btnAPushed();
bool btnADisabled();
bool btnBPushed();
bool btnBDisabled();
//bool btnAnyButtonPressed();
#define ANY_BUTTON_PRESSED (btnAPushed() || btnBPushed())

///////////////////////////////////////// SENSORS       /////////////////////////////////////
// throttle pin
#define PIN_THROTTLE PIN3

extern Servo throttle;
void throttleOff();
void throttlePcnt(unsigned int pcnt);

// micros for 0% throttle eg. delay mode, and for calculations reference
#define THROTTLE_MICROS_MIN 1050
// micros for 100% throttle, too high, and ESC might start to cut out. Theoretical max 2000
#define THROTTLE_MICROS_MAX 1950

// enable voltage sensing
//#define PIN_VOLTAGE A0

// enable current sensing
//#define PIN_CURRENT A1

// enable RPM sensing
//#define PIN_RPM A2

// enable temperature sensing
//#define PIN_TEMP PIN_A3

///////////////////////////////////////// CONFIG METHOD /////////////////////////////////////
// no config. use defaults
// #define CONFIG_NONE

// 5/3/1 pin jumper as on trinket board (5 pins, 3 jumpers, only one active at a time) - 1.5g blue dip switch + solder and glue
// #define CONFIG_JMP4

// read config from 8bit dip switch on continuous pins (eg. 5-12)
/*
 *  12345678
 *  XXXYYRCS
 *
 * XXX - govern by: throttle[%] (default), RPM, or current[A] value
 *      throttle: -13, -7, -3 (75, 78, 82, 85, 88, 91, 95, 98) - when no hold method is selected, a constant throttle
 *          will be applied over the fly time. Not very useful as plane will slow down over time
 *      RPM: 8000, 8500, 9000, 9500, 10000, 10500, 11000, 12000 - set R only. Throttle will be set dynamically to keep
 *          motor RPM constant. Needs previous calibration (see there).
 *      power: 200, 250, 300, 350, 400, 450, 500
 * YY - fly time: (see code)
 * R* - turn on RPM hold and interpret govern as target RPM value - needs rpm sensing
 * C* - turn on power hold and interpret govern as target power (W) - needs voltage and current sensing
 *
 * R+C - set both R+C to turn on SMART throttle
 * S - rotate screen views. To go to another screen, turn on rotate then turn it off when you see it. This view
 *      will be shown by default at next power up. Leave turned on to keep cycling in-flight values. This switch works
 *      in delay ("countdown") and test modes as well, and will reset countdown timer.
 */
#define CONFIG_DIP8

#ifdef CONFIG_DIP8
#define CONFIG_DIP_1 5
#define CONFIG_DIP_2 6
#define CONFIG_DIP_3 7
#define CONFIG_DIP_4 8
#define CONFIG_DIP_5 9
#define CONFIG_DIP_6 10
#define CONFIG_DIP_7 11
#define CONFIG_DIP_8 12
unsigned char readDips(unsigned char cnt);
#endif
// read config from SD card
// #define CONFIG_SD

///////////////////////////////////////// SCREEN        /////////////////////////////////////
// oled screen on default pins A4, A5
#define SCREEN_SSD1306_128X32
#ifdef SCREEN_SSD1306_128X32
#define SCREEN_32X4
#endif
// no screen, timer can still work
// #define SCREEN_NONE

#endif //MONTEMPO_HARDWARE_H
