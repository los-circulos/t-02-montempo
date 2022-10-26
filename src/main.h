#ifndef MONTEMPO_MAIN_H
#define MONTEMPO_MAIN_H

#ifdef CONFIG_DIP_8

#endif

extern uint8_t currentMode;
#define MODE_WELCOME_LOCK 1
#define MODE_WELCOME_COUNTDOWN 2
#define MODE_TEST 10

#define MODE_CONFIG 20
#define MODE_CONFIG_COUNTDOWN 21
#define MODE_DELAY_LOCK 39
#define MODE_DELAY 40
#define MODE_SOFT_START 41
#define MODE_FLY 42
#define MODE_AFTER 50

#define DELAY_COUNTDOWN 200

#endif //MONTEMPO_MAIN_H
