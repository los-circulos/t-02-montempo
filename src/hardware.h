#ifndef MONTEMPO_HARDWARE_H
#define MONTEMPO_HARDWARE_H

#include <Arduino.h>
#include "saved.h"
#include "Servo.h"

#define  BASEV 5.0

void initHardware();

///////////////////////////////////////// LEDS          /////////////////////////////////////
#define LED1 LED_BUILTIN

void ledOn();
void ledOff();

///////////////////////////////////////// BUTTONS       /////////////////////////////////////
#define BTN_A PIN_A6
//#define BTN_B PIN_A7

bool btnAPushed();
bool btnADisabled();
bool btnBPushed();
bool btnBDisabled();
#define ANY_BUTTON_PUSHED anyButtonPushed()
#define ALL_BUTTONS_PUSHED allButtonsPushed()
bool anyButtonPushed();
bool allButtonsPushed();

///////////////////////////////////////// SENSORS       /////////////////////////////////////
// throttle pin
#define PIN_THROTTLE PIN3
// will arm with calibrating throttle range too. Pretty standard in ESCs but just in case.
//  will also save a second, which might (or not) count eg in competition
// comment define to disable (but run at least once having it enabled, so ESC is calibrated)
#define ARM_ON_STARTUP
#define ARM_WITH_CALIBRATE

extern Servo throttle;
void armThrottle();
#define throttleOff() throttle.writeMicroseconds(THROTTLE_MICROS_MIN)
void throttlePcnt(unsigned char pcnt);

// good min/max values but we'll calibrate the throttle anyway
#define THROTTLE_MICROS_MIN 1050
#define THROTTLE_MICROS_MAX 1950

// enable voltage sensing
#define PIN_VOLT A0
//// 18.5V * 4.8 = 88.8 this leaves ~15% margin for high voltage (potmeter set too high) detection
//#define INPUT_DIV_VOLT 4.8
// 18.5V * 4.2 = 890 this leaves ~15% margin for too high voltage (potmeter set too high) detection
// 890 * 1.15 = 1023 which is the highest reading at 5V
#define INPUT_DIV_VOLT 4.2
// volts * 10 over which potmeter is set too low and pin voltage gets near 5V
#define METRICS_V_MAX 212

#ifdef PIN_VOLT
#define VOLTS_DISABLED (saved.voltCut == 0)
#else
#define VOLTS_DISABLED true
#endif

// enable current sensing - currently NOT supported
//#define PIN_CURRENT A1

#ifdef PIN_CURRENT
#define CURRENT_DISABLED (saved.currentCut == 0)
#else
#define CURRENT_DISABLED true
#endif

// enable RPM sensing
#define PIN_RPM PIN2
// @todo make this a meaningful threshold and set rpm 0 if under
#define RPM_MIN 1
#define RPM_MAX 15000

#ifdef PIN_RPM
#define RPM_DISABLED false
#else
#define RPM_DISABLED true
#endif

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
//#define INPUT_DIP8
#define INPUT_DIP9

#ifdef INPUT_DIP8
#define ANY_DIP_INPUT
#define INPUT_DIP_1 5
#define INPUT_DIP_2 6
#define INPUT_DIP_3 7
#define INPUT_DIP_4 8
#define INPUT_DIP_5 9
#define INPUT_DIP_6 10
#define INPUT_DIP_7 11
#define INPUT_DIP_8 12
#define INPUT_DIP_LAST 12
#define INPUT_HOLD_THROTTLE_MULT 2
//#define INPUT_HOLD_RPM_MULT 300
//#define INPUT_HOLD_POWER_MULT 2
unsigned int readDips(unsigned char cnt);
#endif
#ifdef INPUT_DIP9
#define ANY_DIP_INPUT
#define INPUT_DIP_1 4
#define INPUT_DIP_2 5
#define INPUT_DIP_3 6
#define INPUT_DIP_4 7
#define INPUT_DIP_5 8
#define INPUT_DIP_6 9
#define INPUT_DIP_7 10
#define INPUT_DIP_8 11
#define INPUT_DIP_9 12
#define INPUT_DIP_LAST 12
unsigned char readDips(unsigned char cnt);
#define INPUT_HOLD_THROTTLE_MULT 2
//#define INPUT_HOLD_RPM_MULT 300
//#define INPUT_HOLD_POWER_MULT 2
#endif

unsigned char readInputLeft();
unsigned char readInputRight();
unsigned char readInputThrottle();

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
