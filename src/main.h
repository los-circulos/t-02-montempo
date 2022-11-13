#ifndef MONTEMPO_MAIN_H
#define MONTEMPO_MAIN_H

#ifdef INPUT_DIP_8

#endif

extern uint8_t currentMode;
// lock on welcome screen if any button is pressed
// (blink fast)
#define MODE_WELCOME_LOCK 1
// if no button pressed, show home screen and wait 3 secs
#define MODE_WELCOME_COUNTDOWN 2

// saved config input screen
#define MODE_SAVED_INPUT 10
#define MODE_SAVED_INPUT_COUNTDOWN 11
#define MODE_SAVED_INPUT_SAVE 12
#define MODE_SAVED_INPUT_SAVED 14
#define MODE_TEST_SPIN 15

// pre-flight config screen (confirm config)
#define MODE_PREFLIGHT_PROGRAM 20
// when all available buttons pressed (or there are no buttons), wait 3 secs to confirm
//  (and to ignore unintentional button presses)
// (blink fast)
#define MODE_PREFLIGHT_PROGRAM_COUNTDOWN 21
// wait until all available buttons are released
// (blink fast)
#define MODE_DELAY_LOCK 39
// show DELAY countdown (25sec by default)
// (blink slow, then blink fast in the last 5 seconds)
#define MODE_DELAY 40
// raise throttle from 0 to starting value during 5 secs
// (led ON)
#define MODE_SOFT_START 41
// fly (control motor) based on previous program
// (led on, flashes fast in last 5 seconds)
#define MODE_FLY 42
// blink fast but with extra delay, show in-flight data for recording etc
#define MODE_AFTER 50

//#define DELAY_COUNTDOWN 150
#define DELAY_COUNTDOWN 100
#define DELAY_COUNTDOWN_FAST 100

#endif //MONTEMPO_MAIN_H
