#include "hardware.h"
#include "config.h"

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

void throttleOff() {
//    throttle.writeMicroseconds((THROTTLE_MICROS_MIN + THROTTLE_MICROS_MAX) / 2);
    throttle.writeMicroseconds(THROTTLE_MICROS_MIN);
}

void throttlePcnt(unsigned int pcnt) {
    unsigned int i = THROTTLE_MICROS_MIN + (THROTTLE_MICROS_MAX - THROTTLE_MICROS_MIN) * pcnt / 100;
    throttle.writeMicroseconds(i);
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