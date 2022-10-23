#ifndef MONTEMPO_TIME_H
#define MONTEMPO_TIME_H

extern unsigned long currentTime;

#define BLINKDIV_ERR 100
#define BLINKDIV_FAST 250
#define BLINKDIV_NORMAL 500
#define BLINKDIV_SLOW 1000

bool errorBlink();

#endif //MONTEMPO_TIME_H
