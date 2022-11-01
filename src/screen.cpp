#include "config.h"
#include "hardware.h"
#include "screen.h"
#include "mytime.h"
#include "saved.h"

////////////////////////////////////////////////////////////////////////////////
// screen hardware specific START
////////////////////////////////////////////////////////////////////////////////
#ifdef SCREEN_32X4

U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED

#define SET_FONT_XL u8x8.setFont(FONT_XL)
#define SET_FONT_L u8x8.setFont(FONT_L)
#define SET_FONT_S u8x8.setFont(FONT_S)

#endif
////////////////////////////////////////////////////////////////////////////////
// screen hardware specific END
////////////////////////////////////////////////////////////////////////////////

int currentScreen = SCREEN_PRE;

// should never be more than 17, but let's play safe
char buffer[20];
char floatBuffer[6];

char *testModeLabels[] = {"MOTOR", "SMART", "T1CUT", "T2CUT", "V CUT", "A CUT", "MODE ", "POLES"};
char testModeUnits[] = "%%CCVA P";
char *holdModeLabels[] = {"THRO", "RPM ", "PWR ", "SMRT"};

#define FMT_TEST_VALUE_COMMON " %2d%c"
#define FMT_TEST_VALUE_COMMON " %2d%c"
#define FMT_TEST_VALUE_VOLTS "3.%1dV"
#define FMT_TEST_VALUE_COMMON " %2d%c"
#define FMT_TEST_4DECIMALS "%4d"

void initScreen() {
#ifdef SCREEN_32X4
    u8x8.begin();
    u8x8.setPowerSave(0);
#endif
}
void clearScreen() {
#ifdef SCREEN_32X4
    u8x8.clear();
#endif
}

void drawArming() {
#ifdef SCREEN_32X4
    SET_FONT_XL;
    u8x8.drawString(0, 0, "ARMING..");
#endif
}
void drawFlyConfirmation(bool show) {
#ifdef SCREEN_32X4
    SET_FONT_XL;
    if (show) {
        u8x8.drawString(14, 0, "!");
    }
    else {
        u8x8.drawString(14, 0, " ");
    }
#endif
}
void drawLogoLock() {
#ifdef SCREEN_32X4
    SET_FONT_S;
    if (blinkLed(BLINK_FAST)) {
        u8x8.drawString(4, 3, " !REL! ");
    }
    else {
        eraseLogoLock();
    }
#endif
}
void eraseLogoLock() {
#ifdef SCREEN_32X4
    u8x8.drawString(4, 3, "       ");
#endif
}
void drawNotImplemented() {
    if (blinkLed(BLINK_FAST)) {
#ifdef SCREEN_32X4
        SET_FONT_XL;
        u8x8.drawString(0, 1, "NOT YET");
#endif
    }
    else {
        clearScreen();
    }
}
void drawRemainingTime(unsigned int secsRemain) {
    sprintf(
            buffer,
            "%02d%s%02d",
            secsRemain/60,
            blinkLed(secsRemain < 5 ? BLINK_FAST : BLINK_NORMAL) ? "." : " ",
            secsRemain % 60
    );
#ifdef SCREEN_32X4
    SET_FONT_XL;
    u8x8.drawString(3, 0, buffer);
#endif
}
void drawSaved() {
    clearScreen();
#ifdef SCREEN_32X4
    SET_FONT_XL;
    u8x8.drawString(0, 0, "SAVED!  ");
#endif
//    drawLogoLock();
}
void drawWaitDot(uint8_t x) {
#ifdef SCREEN_32X4
    SET_FONT_S;
    u8x8.drawString(x + 3, 3, ".");
#endif
}

void drawWelcome() {
    clearScreen();
#ifdef SCREEN_32X4
    SET_FONT_XL;
    u8x8.drawString(0, 0, "MONTEMPO");
    SET_FONT_S;
    u8x8.drawString(0, 3, "F2B");
    u8x8.drawString(12, 3, "V1.0");
#endif
}
void drawPreflight(configT config) {
#ifdef SCREEN_32X4

    SET_FONT_L;

    u8x8.drawString(10, 0, "FLY?");

    // NOTE a switch would eat 4bytes more here
    if (saved.holdMode == HOLD_MODE_RPM) {
        sprintf(buffer, "RPM %2d.%d", config.RPM/1000, (config.RPM/100)%10);
        u8x8.drawString(0, 2, buffer);
    }
    else if (saved.holdMode == HOLD_MODE_POWER) {
        sprintf(buffer, "PWR  %3d", config.power);
        u8x8.drawString(0, 2, buffer);
    }
    else if (saved.holdMode == HOLD_MODE_SMART_THROTTLE) {
        sprintf(buffer, "SMRT %2d%%", config.throttle);
        u8x8.drawString(0, 2, buffer);
    }
    else {
        sprintf(buffer, "THRO %2d%%", config.throttle);
        u8x8.drawString(0, 2, buffer);
    }

    float u = 12.456;
    dtostrf(u, 2, 2, floatBuffer);
    sprintf(buffer, "V  %s", floatBuffer);
    u8x8.drawString(0,0, buffer);

    sprintf(buffer, "%2d:%02d", config.timeFly / 60, config.timeFly % 60);
    u8x8.drawString(9, 2, buffer);

#endif
}
void drawTestScreen() {

    switch (testMode) {
    case TESTMODE_SPIN:
    case TESTMODE_SMART:
    case TESTMODE_T1_CUT:
    case TESTMODE_T2_CUT:
    case TESTMODE_CURRENT_CUT:
    case TESTMODE_POLES:
        sprintf(buffer, FMT_TEST_VALUE_COMMON, testValue, testModeUnits[testMode]);
        break;
    case TESTMODE_VOLT_CUT:
        sprintf(buffer, FMT_TEST_VALUE_VOLTS, testValue);
        sprintf(floatBuffer, FMT_TEST_VALUE_VOLTS, saved.voltCut);
        break;
    case TESTMODE_MODE:
        strcpy(buffer, holdModeLabels[testValue]);
        strcpy(floatBuffer, holdModeLabels[saved.holdMode]);
    // omitting this saves 30 bytes but testMode rather be valid then lol
//        break;
//    default:
//        sprintf(buffer, "USED");
//        sprintf(floatBuffer, "----");
    }
    switch (testMode) {
    case TESTMODE_SPIN:
        strcpy(floatBuffer, "    ");
        break;
    case TESTMODE_SMART:
        sprintf(floatBuffer, FMT_TEST_4DECIMALS, saved.smartEndThrottle);
        break;
    case TESTMODE_T1_CUT:
        sprintf(floatBuffer, FMT_TEST_VALUE_COMMON, saved.t1Cut, testModeUnits[testMode]);
        break;
    case TESTMODE_T2_CUT:
        sprintf(floatBuffer, FMT_TEST_VALUE_COMMON, saved.t2Cut, testModeUnits[testMode]);
        break;
    case TESTMODE_CURRENT_CUT:
        sprintf(floatBuffer, FMT_TEST_VALUE_COMMON, saved.currentCut, testModeUnits[testMode]);
        break;
    case TESTMODE_POLES:
        sprintf(floatBuffer, FMT_TEST_4DECIMALS, saved.poles);
        break;
    }

#ifdef SCREEN_32X4

    SET_FONT_L;
    u8x8.drawString(0, 0, testModeLabels[testMode]);
    u8x8.drawString(0, 2, floatBuffer);

    SET_FONT_XL;
    u8x8.drawString(6, 0, buffer);

    SET_FONT_S;
    if (testMode == TESTMODE_SPIN) {
//        u8x8.drawString(0, 2, "    ");
        u8x8.drawString(4, 3, "PUSH TO SPIN");
    }
    else {
        u8x8.drawString(4, 3, " <OLD  ^SAVE");
    }

#endif
}

void drawRunScreen() {
#ifdef SCREEN_32X4
    SET_FONT_L;
//    u8x8.drawString(0, 0, "THR __  V __._");
    sprintf(buffer, "THR %2d  V __._", testValue);
    u8x8.drawString(0, 0, buffer);
    //    u8x8.drawString(0, 2, "RPM ____ A ___");

//    u8x8.drawString(0, 0, "THR __ PWR ___");
//    u8x8.drawString(0, 1, "T1__ T2__ P___");

#endif
}
