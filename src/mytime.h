#ifndef MONTEMPO_MYTIME_H
#define MONTEMPO_MYTIME_H

extern unsigned long currentTime;

// millis / 10 (so they fit in a uint8_t) eg. 50 will blink once each second (50*10 = 500 so 500 millis on, 500 millis off)
#define BLINK_ERR 5
#define BLINK_FAST 10
#define BLINK_NORMAL 50
#define BLINK_SLOW 100

void setCurrentTime();
bool blinkLed(uint8_t t);

#endif //MONTEMPO_MYTIME_H
