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

// for now the macro version uses less memory. leaving here as if it gets reused many times it might make sense to use a function
//bool btnAnyButtonPressed() {
//    return btnAPushed() || btnBPushed();
//}
