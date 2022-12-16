#include "hardware.h"
#include "config.h"
#include "metrics.h"

//volatile unsigned int rpmCnt = 0;
//unsigned long rpmConvertLast = 0;
//// don't convert if last conversion happened more than 6 seconds ago. A 20pole motor would take over 15k rpm to get
////  the rpm buffer overflow (given there's no base freq value to subtract)
//#define MAX_CONVERT_MILLIS 5000
//void rpmISR() {
//    rpmCnt++;
//}

void initHardware() {

    pinMode(LED1, OUTPUT);

    // NOTE on the nano these won't work with default pins A6, A7 and these two pins need a pullup resistor
    if (!btnADisabled()) {
        pinMode(BTN_A, INPUT_PULLUP);
    }
#ifdef BTN_B
    if (!btnBDisabled()) {
        pinMode(BTN_B, INPUT_PULLUP);
    }
#endif

#ifdef ANY_DIP_INPUT
    for (int i=INPUT_DIP_1; i <= INPUT_DIP_LAST; i++) {
        pinMode(i, INPUT_PULLUP);
    }
#endif

//#ifdef PIN_VOLT
//    pinMode(PIN_VOLT, INPUT);
//#endif

//#ifdef PIN_CURRENT
//    pinMode(PIN_CURRENT, INPUT);
//#endif

#ifdef PIN_RPM
//    pinMode(PIN_RPM, INPUT);
//    attachInterrupt(digitalPinToInterrupt(PIN_RPM), rpmISR, RISING);
#endif

}

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
#ifdef BTN_B
    return analogRead(BTN_B) IS_PUSHED_BTN;
#else
    return false;
#endif
}
bool btnBDisabled() {
#ifdef BTN_B
    int i = analogRead(BTN_B);
    return !(i IS_PUSHED_BTN) && (i IS_DISABLED_BTN);
#else
    return true;
#endif
}

bool allButtonsPushed() {
    return btnAPushed() && (btnBDisabled() || btnBPushed());
}
bool anyButtonPushed() {
    return btnAPushed() || (!btnBDisabled() && btnBPushed());
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

void throttlePcnt(unsigned char pcnt) {
    // order matters as overflow could happen during first version computation
//    unsigned long i = THROTTLE_MICROS_MIN + (THROTTLE_MICROS_MAX - THROTTLE_MICROS_MIN) * pcnt / 100;
    // @todo just unsigned int?
    unsigned long i = (THROTTLE_MICROS_MAX - THROTTLE_MICROS_MIN)/100 * pcnt + THROTTLE_MICROS_MIN;
    throttle.writeMicroseconds(i);
    metrics.throttlePcnt = pcnt;
    // test code - might need it someday
//    sprintf(tmp, "%d %d ", pcnt, i);
//    u8x8.drawString(0, 2, tmp );
}
void convertRpm() {

}

#ifdef ANY_DIP_INPUT
unsigned char readDips(unsigned char cnt) {
    unsigned char i, v = 1;
    unsigned char ret = 0;
    for (i=0; i<cnt; i++) {
        ret += !digitalRead(INPUT_DIP_1 + i) * v;
        v *= 2;
    }
    return ret;
}
#endif