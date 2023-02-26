#include "hardware.h"
#include "config.h"
#include "metrics.h"
#include "screen.h"

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

#ifdef PIN_THROTTLE

    // this will leave the ESC initialized but not armed - not true for at least one old dualsky ESC, which arms on 0 signal as well
    throttle.attach(PIN_THROTTLE, THROTTLE_MICROS_MIN, THROTTLE_MICROS_MAX);

    // @todo arm only when not in saved config mode?
    if (saved.armOnBoot) {
        armThrottle();
    }
    else {
        throttle.writeMicroseconds(0);
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

    drawArming();

#ifdef PIN_THROTTLE

    if (!throttleArmed) {

        if (saved.calibrate) {
            // this will leave a 1sec delay to calibrate throttle min/max values
            throttlePcnt(99);
            delay(1000);
        }

    }
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

#ifdef ANY_DIP_INPUT
unsigned char readInputLeft() {
    return digitalRead(INPUT_DIP_1) * 8 +
            digitalRead(INPUT_DIP_2) * 4 +
            digitalRead(INPUT_DIP_3) * 2 +
            digitalRead(INPUT_DIP_4) * 1;
}
#endif

#ifdef INPUT_DIP8
//unsigned char readDips(unsigned char cnt) {
unsigned int readDips(unsigned char cnt) {
//    unsigned char i, v = 2;
    unsigned char v = 2;
    unsigned int ret = 0;
    for (unsigned char i=1; i<cnt; i++) {
        ret += !digitalRead(INPUT_DIP_1 + i - 1) * v;
        v *= 2;
    }
    return ret;
}
unsigned char readInputRight() {
    return digitalRead(INPUT_DIP_5) * 16 +
            digitalRead(INPUT_DIP_6) * 8 +
            digitalRead(INPUT_DIP_7) * 4 +
            digitalRead(INPUT_DIP_8) * 2;
}
unsigned char readInputThrottle() {
    return 98 - digitalRead(INPUT_DIP_5) * 32 -
                digitalRead(INPUT_DIP_6) * 16 -
                digitalRead(INPUT_DIP_7) * 8 -
                digitalRead(INPUT_DIP_8) * 4;
}
#endif
#ifdef INPUT_DIP9
unsigned char readInputRight() {
    return digitalRead(INPUT_DIP_5) * 16 +
            digitalRead(INPUT_DIP_6) * 8 +
            digitalRead(INPUT_DIP_7) * 4 +
            digitalRead(INPUT_DIP_8) * 2 +
            digitalRead(INPUT_DIP_9) * 1;
}
unsigned char readInputThrottle() {
    return 98 - (digitalRead(INPUT_DIP_5) * 16 +
                digitalRead(INPUT_DIP_6) * 8 +
                digitalRead(INPUT_DIP_7) * 4 +
                digitalRead(INPUT_DIP_8) * 2 +
                digitalRead(INPUT_DIP_9) * 1) * 2;
}
#endif
