#include "config.h"
#include "hardware.h"
#include "screen.h"
#include "mytime.h"
#include "saved.h"
#include "metrics.h"

////////////////////////////////////////////////////////////////////////////////
// screen hardware specific START
////////////////////////////////////////////////////////////////////////////////
#ifdef SCREEN_32X4

U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED

#define SET_FONT_XXL u8x8.setFont(FONT_XXL)
#define SET_FONT_XL u8x8.setFont(FONT_XL)
#define SET_FONT_L u8x8.setFont(FONT_L)
#define SET_FONT_S u8x8.setFont(FONT_S)

#endif
////////////////////////////////////////////////////////////////////////////////
// screen hardware specific END
////////////////////////////////////////////////////////////////////////////////

unsigned char currentScreen = SCREEN_PRE;

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
    if (blinkLed(BLINK_ERR)) {
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
//    char c = blinkLed(secsRemain < 5 ? BLINK_FAST : BLINK_NORMAL) ? ' ' : '.';
//    blinkLed(secsRemain < 5 ? BLINK_FAST : BLINK_SLOW);
#ifdef SCREEN_32X4
//    // @todo this shouldn't be here but in calling method?
//    if (secsRemain % 2 == 0) {
//        u8x8.inverse();
//    }
//    else {
//        u8x8.noInverse();
//    }
    if (secsRemain > 59) {
        sprintf(
                buffer,
                "%2d.%02d",
                secsRemain / 60,
                secsRemain % 60
        );
    }
    else {
        sprintf(
                buffer,
                "  .%2d",
                secsRemain % 60
        );
    }

    SET_FONT_XL;
    u8x8.drawString(6, 0, buffer);
//    sprintf(buffer, "%c", c);
//    sprintf(buffer, "%c", (secsRemain % 2 > 0 ? ' ' : '.'));
//    u8x8.drawString(11, 0, buffer);
//    u8x8.drawString(11, 1, buffer);
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

    if (metrics.volts > METRICS_V_MAX) {
        sprintf(buffer, "V OVER!");
    }
    else {
        sprintf(buffer, "V  %4d", metrics.volts);
    }
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
void drawTestRunScreen() {
#ifdef SCREEN_32X4
    SET_FONT_L;
//    u8x8.drawString(0, 0, "THR __  V __._");
    sprintf(buffer, "THR %2d  V %2d.%1d", testValue, metrics.volts/10, metrics.volts%10);
    u8x8.drawString(0, 0, buffer);
//    sprintf(buffer,  "RPM ____ A %2d.%1d", metrics.amps/10, metrics.amps%10);
//    sprintf(buffer,  "RPM ____ A %4d", metrics.amps);
    sprintf(buffer,  "RPM %4d A %4d", metrics.rpm, metrics.amps);
    u8x8.drawString(0, 2, buffer);

//    u8x8.drawString(0, 0, "THR __ PWR ___");
//    u8x8.drawString(0, 1, "T1__ T2__ P___");

#endif
}
void drawRunScreen(int secsRemain) {

    drawRemainingTime(secsRemain);

#ifdef SCREEN_32X4

//    SET_FONT_S;

    if (metrics.volts > 0) {
//        sprintf(buffer, "%2d.%1d V", metrics.volts/10, metrics.volts%10);
        sprintf(buffer, "%2d %1d", metrics.volts/10, metrics.volts%10);
        sprintf(&buffer[10], "V");
    }
    else {
        sprintf(buffer, "     ");
        sprintf(&buffer[10], " ");
    }
    SET_FONT_L;
    u8x8.drawString(0, 0, buffer);
    SET_FONT_S;
    u8x8.drawString(2, 1, &buffer[10]);

    if (metrics.amps > 0) {
        sprintf(buffer, "%2d.%1dA", metrics.amps/10, metrics.amps%10);
    }
    else {
        sprintf(buffer, "     ");
    }
    SET_FONT_L;
    u8x8.drawString(0, 2, buffer);

    // @todo temps

#endif

}
void drawAfterScreen(unsigned char which) {

    clearScreen();

#ifdef SCREEN_32X4
    SET_FONT_L;
    if (which == 0) {
        u8x8.drawString(0, 0, "# 41");
        SET_FONT_XL;
        u8x8.drawString(4, 0, "!OOPS!");
        SET_FONT_S;
        u8x8.drawString(0, 3, "2:32  T1CUT  62C");
    }
    else if (which == 1) {
//        SET_FONT_L;
        u8x8.drawString(0, 0, "# 43");
        u8x8.drawString(0, 2, "5:11");
        u8x8.drawString(13, 1, "MAH");
        SET_FONT_XL;
        u8x8.drawString(5, 0, "2200");
        SET_FONT_S;
//        u8x8.drawString(13, 0, "!T1");
        u8x8.drawString(13, 0, " OK");
        // formula power: P = 10*t/c
        u8x8.drawString(4, 3, "S 12.4V 325W");
    }
    else if (which == 2) {
//        SET_FONT_L;
        u8x8.drawString(0, 0, "V  14.8 ... 16.8");
        u8x8.drawString(0, 2, "A   20   25   50");
    }
    else if (which == 3) {
//        SET_FONT_L;
        u8x8.drawString(0, 0, "P  250  300  320");
        u8x8.drawString(0, 2, "R 11.1 12.5 13.1");
    }
    else if (which == 4) {
//        SET_FONT_L;
        u8x8.drawString(0, 0, "T1  16   32   34");
        u8x8.drawString(0, 2, "T2  16   32   34");
    }
    /*
    else if (which == 5) {
//        SET_FONT_L;
        u8x8.drawString(0, 0, "14.8 V  1800 MAH");
        u8x8.drawString(0, 2, "5:11 11C 33C 22C");
    }
    else if (which == 6) {
//        SET_FONT_L;
        u8x8.drawString(0, 0, "# 42");
        u8x8.drawString(0, 2, "5:11");
        u8x8.drawString(13, 1, "MAH");
        SET_FONT_XL;
        u8x8.drawString(5, 0, "2042");
        SET_FONT_S;
        u8x8.drawString(13, 0, "!T1");
//        u8x8.drawString(13, 0, " OK");
        // formula power: P = 10*t/c
        u8x8.drawString(4, 3, "T 12.4V 236W");
    }
     */
    else {
        SET_FONT_XL;
        u8x8.drawString(0, 0, "WELLDONE");
        SET_FONT_S;
        u8x8.drawString(0, 3, "   @LOS.CIRCULOS");
    }
#endif
}
