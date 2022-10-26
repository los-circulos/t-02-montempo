#ifndef MONTEMPO_HARDWARE_H
#define MONTEMPO_HARDWARE_H

#include <Arduino.h>

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
 *      current: 13,18,21,24,28,33,37,45 - set C only.
 * YY - fly time: 60s, 180s, 300s, 360s
 * R* - turn on RPM hold and interpret govern as target RPM value
 * C* - turn on current hold and interpret govern as target amps
 * R+C - * = set both R+C on to enter test mode
 * S - rotate screen views. To go to another screen, turn on rotate then turn it off when you see it. This view
 *      will be shown by default at next power up. Leave turned on to keep cycling in-flight values.
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
#endif
// read config from SD card
// #define CONFIG_SD

///////////////////////////////////////// SCREEN        /////////////////////////////////////
// oled screen on default pins A4, A5
#define SCREEN_SSD1306_128X32
// no screen, timer can still work
// #define SCREEN_NONE

#endif //MONTEMPO_HARDWARE_H
