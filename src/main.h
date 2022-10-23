#ifndef MONTEMPO_MAIN_H
#define MONTEMPO_MAIN_H

#ifdef CONFIG_DIP_8

#endif

extern int currentMode;
#define MODE_LOGO_LOCK 0
#define MODE_LOGO 1
#define MODE_TEST 10
#define MODE_CONFIG 20
#define MODE_DELAY 40
#define MODE_FLY 42


void trapWhileAnyButtonPressed();

#endif //MONTEMPO_MAIN_H
