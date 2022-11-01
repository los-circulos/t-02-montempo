#include "hardware.h"

void ledOn() {
    digitalWrite(LED1, HIGH);
}
void ledOff() {
    digitalWrite(LED1, LOW);
}

#define IS_PUSHED_BTN < 50
#define IS_DISABLED_BTN < 200

bool btnAPushed() {
    return analogRead(BTN_A) IS_PUSHED_BTN;
}
bool btnADisabled() {
    int i = analogRead(BTN_A);
    return !(i IS_PUSHED_BTN) && (i IS_DISABLED_BTN);
}

bool btnBPushed() {
    return analogRead(BTN_B) IS_PUSHED_BTN;
}
bool btnBDisabled() {
    int i = analogRead(BTN_B);
    return !(i IS_PUSHED_BTN) && (i IS_DISABLED_BTN);
}

Servo throttle = Servo();
bool throttleArmed = false;

void armThrottle() {
#ifdef ARM_WITH_CALIBRATE
    // this will leave a 1sec delay but also calibrate min/max values
    if (!throttleArmed) {
        throttlePcnt(99);
        delay(1000);
    }
    throttleOff();
#else
    // might not be compatible with all ESCs
    throttleOff();
#endif
    throttleArmed = true;
}

void throttlePcnt(unsigned int pcnt) {
    // order matters as overflow could happen during first version computation
//    unsigned long i = THROTTLE_MICROS_MIN + (THROTTLE_MICROS_MAX - THROTTLE_MICROS_MIN) * pcnt / 100;
    // @todo just unsigned int?
    unsigned long i = (THROTTLE_MICROS_MAX - THROTTLE_MICROS_MIN)/100 * pcnt + THROTTLE_MICROS_MIN;
    throttle.writeMicroseconds(i);
    // test code - might need it someday
//    sprintf(tmp, "%d %d ", pcnt, i);
//    u8x8.drawString(0, 2, tmp );
}

#ifdef CONFIG_DIP8
unsigned char readDips(unsigned char cnt) {
    unsigned char i, v = 1;
    unsigned char ret = 0;
    for (i=0; i<cnt; i++) {
        ret += !digitalRead(CONFIG_DIP_1+i) * v;
        v *= 2;
    }
    return ret;
}
#endif