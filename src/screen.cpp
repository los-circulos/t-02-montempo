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
char floatBuffer[10];
char progressBarChars[4] = {'>','#','#',' '};

char *savedInputModeLabels[] = {"MOTOR", "SMART", "T1CUT", "T2CUT", "V CUT", "A CUT", "MODE ", "POLES"};
char savedInputModeUnits[] = "%%CCVA P";
char *holdModeLabels[] = {"THRO", "RPM ", "PWR ", "SMRT"};
// @todo more modes - simulate glow with stunt tank, with uniflow tank, and erratic run with hole on tank
//char *holdModeLabels[] = {"THRO", "RPM ", "PWR ", "SMRT", "GLOW", "UNIF", "ERRA"};
//char *resultLabels[] = {"OKTIME", "OKVCUT", "BTN", "V ???", "VHIGH", "A ???", "AHIGH", "R??", "RHIGH", "T1???", "T2???"};
char *resultLabels[] = {"OK T", "OK V", "BTN", "V?", "VHI", "A?", "R?", "RHI", "T1?", "T2?"};
char *comingSoon = "COMING SOON";

#define FMT_TEST_VALUE_COMMON " %2d%c"
#define FMT_TEST_VALUE_VOLTS "3.%1dV"
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

void sprintFixedTimeAndResult() {
    sprintf(buffer, "%01d:%02d", metricsSum.flightTime/60, metricsSum.flightTime%60);
// leaving these coupled for now, saves only 4byte per call eliminated but increases coupling ( = separate it if
//  this coupling causes too much problems)
//}
//void sprintResult() {
    if (metricsSum.result < REULTS_ERRORS_CUTS_FROM) {
        sprintf(floatBuffer, "%s", resultLabels[metricsSum.result]);
    }
    else {
        sprintf(floatBuffer, "%6s", savedInputModeLabels[metricsSum.result - REULTS_ERRORS_CUTS_FROM + 2]);
    }
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
void drawNoFly() {
#ifdef SCREEN_32X4
    SET_FONT_XL;
    u8x8.drawString(0, 0, "NO FLY !");
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
//    blinkLed(secsRemain < 5 ? BLINK_FAST : BLINK_SLOW);
#ifdef SCREEN_32X4
    if (secsRemain > 59) {
        sprintf(
                buffer,
//                "%2d.%02d",
                "%1d.%02d",
                secsRemain / 60,
                secsRemain % 60
        );
    }
    else {
        sprintf(
                buffer,
//                "  .%2d",
                " .%2d",
                secsRemain % 60
        );
    }

//    SET_FONT_XXL;
//    u8x8.drawString(1, 0, buffer);
    SET_FONT_XL;
//    u8x8.drawString(3, 0, buffer);
    u8x8.drawString(8, 0, buffer);
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
void draw3Decimals(char unit, unsigned int v1, unsigned int v2, unsigned int v3) {
    sprintf(buffer, "%c %2d.%1d %2d.%1d %2d.%1d",
            unit,
            v1 / 10, v1 % 10,
            v2 / 10, v2 % 10,
            v3 / 10, v3 % 10
    );
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

    // NOTE a switch would eat 4bytes more here than these ifs
    if (saved.holdMode == HOLD_MODE_RPM) {
        sprintf(buffer, "RPM %2d.%d", config.holdRPM / 1000, (config.holdRPM / 100) % 10);
        u8x8.drawString(0, 2, buffer);
    }
    else if (saved.holdMode == HOLD_MODE_POWER) {
        sprintf(buffer, "PWR  %3d", config.holdPower);
        u8x8.drawString(0, 2, buffer);
    }
    else if (saved.holdMode == HOLD_MODE_SMART_THROTTLE) {
        sprintf(buffer, "SMR %2d-%2d", config.holdThrottle, saved.smartEndThrottle);
        u8x8.drawString(0, 2, buffer);
    }
    else {
        sprintf(buffer, "THRO %2d%%", config.holdThrottle);
        u8x8.drawString(0, 2, buffer);
    }

    config.preflightError = true;
    if (metrics.volts > METRICS_V_MAX) {
        sprintf(buffer, "V HIGH!!");
    }
    else if (config.cellCount == CONFIG_CELLS_ERR_VCUT) {
        sprintf(buffer, "VCUT ERR");
    }
    else if (config.cellCount == CONFIG_CELLS_ERR_VLOW) {
        sprintf(buffer, "V LOW !!");
    }
    else {
        sprintf(buffer, "%2d.%1dV %1dS", metrics.volts/10, metrics.volts%10, config.cellCount);
        config.preflightError = false;
    }
    u8x8.drawString(0,0, buffer);

    if (
// not possible without VOLT measuring:
#ifndef PIN_VOLT
        // flytime 0 would fly until Vcut
        (config.timeFly == 0) ||
        // power calculation needs volts (and current)
        (saved.holdMode == HOLD_MODE_POWER) ||
#else
#ifndef PIN_CURRENT
        // power calculation needs current (and volts)
        (saved.holdMode == HOLD_MODE_POWER) ||
#endif
#endif
        (   (saved.holdMode == HOLD_MODE_SMART_THROTTLE) &&
            (
                // smart throttle end value should be bigger than start
                (config.holdThrottle > saved.smartEndThrottle) ||
                // smart throttle cannot calculate until vcut (at least for now)
                (config.timeFly == 0)
            )
        )
    ) {
        config.preflightError = true;
    }

    if (config.preflightError) {
        u8x8.drawString(10, 0, " NO ");
        u8x8.drawString(9, 2, " FLY!");
    }
    else {
        u8x8.drawString(10, 0, "FLY?");
        sprintf(buffer, "%2d:%02d", config.timeFly / 60, config.timeFly % 60);
        u8x8.drawString(9, 2, buffer);
    }

#endif

}
void drawSavedInputScreen() {

    switch (savedInputMode) {
    case SAVED_INPUT_MODE_SPIN:
    case SAVED_INPUT_MODE_SMART:
    case SAVED_INPUT_MODE_T1_CUT:
    case SAVED_INPUT_MODE_T2_CUT:
    case SAVED_INPUT_MODE_CURRENT_CUT:
    case SAVED_INPUT_MODE_POLES:
        sprintf(buffer, FMT_TEST_VALUE_COMMON, savedInputValue, savedInputModeUnits[savedInputMode]);
        break;
    case SAVED_INPUT_MODE_VOLT_CUT:
        sprintf(buffer, FMT_TEST_VALUE_VOLTS, savedInputValue);
        sprintf(floatBuffer, FMT_TEST_VALUE_VOLTS, saved.voltCut);
        break;
    case SAVED_INPUT_MODE_MODE:
        strcpy(buffer, holdModeLabels[savedInputValue]);
        strcpy(floatBuffer, holdModeLabels[saved.holdMode]);
    // omitting this saves 30 bytes but savedInputMode rather be valid then lol
//        break;
//    default:
//        sprintf(buffer, "USED");
//        sprintf(floatBuffer, "----");
    }
    switch (savedInputMode) {
    case SAVED_INPUT_MODE_SPIN:
        strcpy(floatBuffer, "    ");
        break;
    case SAVED_INPUT_MODE_SMART:
        sprintf(floatBuffer, FMT_TEST_4DECIMALS, saved.smartEndThrottle);
        break;
    case SAVED_INPUT_MODE_T1_CUT:
        sprintf(floatBuffer, FMT_TEST_VALUE_COMMON, saved.t1Cut, savedInputModeUnits[savedInputMode]);
        break;
    case SAVED_INPUT_MODE_T2_CUT:
        sprintf(floatBuffer, FMT_TEST_VALUE_COMMON, saved.t2Cut, savedInputModeUnits[savedInputMode]);
        break;
    case SAVED_INPUT_MODE_CURRENT_CUT:
        sprintf(floatBuffer, FMT_TEST_VALUE_COMMON, saved.currentCut, savedInputModeUnits[savedInputMode]);
        break;
    case SAVED_INPUT_MODE_POLES:
        sprintf(floatBuffer, FMT_TEST_4DECIMALS, saved.poles);
        break;
    }

#ifdef SCREEN_32X4

    SET_FONT_L;
    u8x8.drawString(0, 0, savedInputModeLabels[savedInputMode]);
    u8x8.drawString(0, 2, floatBuffer);

    SET_FONT_XL;
    u8x8.drawString(6, 0, buffer);

    SET_FONT_S;
    if (savedInputMode == SAVED_INPUT_MODE_SPIN) {
//        u8x8.drawString(0, 2, "    ");
        u8x8.drawString(4, 3, "PUSH TO SPIN");
    }
    else {
        u8x8.drawString(4, 3, " <OLD  ^SAVE");
    }

#endif
}
void drawTestSpinScreen() {
#ifdef SCREEN_32X4
    SET_FONT_L;
    sprintf(buffer, "THR %2d %% %2d.%1d V", savedInputValue, metrics.volts / 10, metrics.volts % 10);
    u8x8.drawString(0, 0, buffer);
//    sprintf(buffer,  "RPM %2d.%1d %4d A", metrics.rpm/1000, (metrics.rpm%1000)/100, metrics.amps);
    sprintf(buffer,  "RPM %4d %4d A", metrics.rpm, metrics.amps);
    u8x8.drawString(0, 2, buffer);
#endif
}
void drawRunScreen(unsigned int secsRemain) {

#ifdef SCREEN_32X4

    // flash current value with inverse when not rotating screens. Not sure if it's of any use.
    // indeed only useful when there is no visible in-flight led but the screen is visible
//    if (!config.rotateScreens) {
    // plan B: flash value when flying until battery time
//    if ((config.timeFly == 0) && (secsRemain % 2 == 0)) {
//        u8x8.inverse();
//    }
//    else {
//        u8x8.noInverse();
//    }
    // for now, no inverse play
//    u8x8.noInverse();

    drawRemainingTime(secsRemain);

    // draw progress
    if (config.timeFly > 0) {
        // timeFly/16 = (16-secsRemain)/x
        // x = 64 - secsRemain*64/timeFly -1 (-1 so printing won't overflow on secsRemain=0)
//        secsRemain = 63 - secsRemain*64/config.timeFly;
//        secsRemain = 43 - secsRemain*44/config.timeFly;
        secsRemain = 59 - secsRemain*60/config.timeFly;

        unsigned int i = secsRemain % 4;

        buffer[0] = progressBarChars[i];
        buffer[1] = 0;

        if (i > 1) {
            u8x8.inverse();
        }
        else {
            u8x8.noInverse();
        }

//        i = secsRemain / 4 + 5;
        i = secsRemain / 4 + 1;

        SET_FONT_S;
        u8x8.drawString(i, 3, buffer);

        u8x8.inverse();
//        for (i--; i>5; i--) {
        for (i--; i>1; i--) {
            u8x8.drawString(i, 3, " ");
        }

    }

    u8x8.noInverse();
#ifdef DEVMODE_

    SET_FONT_S;

//    sprintf(buffer, "%4d", sizeof(metricsSumCntT));
//    u8x8.drawString(6, 3, buffer);

//    sprintf(buffer, "%4d", metrics.amps*2);
//    u8x8.drawString(0, 0, buffer);
//    sprintf(buffer, "%4d", metricsSum.ampsMin*2);
//    u8x8.drawString(0, 1, buffer);
//    sprintf(buffer, "%4d", (int)metricsSum.ampsAvg*2);
//    u8x8.drawString(0, 2, buffer);
//    sprintf(buffer, "%4d", metricsSum.ampsMax*2);
//    u8x8.drawString(0, 3, buffer);

//    sprintf(buffer, "%4d", metrics.volts);
//    u8x8.drawString(0, 0, buffer);
//    sprintf(buffer, "%4d", metricsSum.voltsMin);
//    u8x8.drawString(0, 1, buffer);
////    sprintf(buffer, "%4d", (int)METRICS_AVG_AMPS*2);
////    u8x8.drawString(0, 2, buffer);
//    sprintf(buffer, "%4d", metricsSum.voltsMax);
//    u8x8.drawString(0, 3, buffer);

    sprintf(buffer, "%4d%%", metrics.throttlePcnt);
    u8x8.drawString(0, 0, buffer);
    sprintf(buffer, "%3d", metrics.volts);
//    sprintf(buffer, "%3dV", saved.smartEndThrottle);
    u8x8.drawString(0, 1, buffer);
    sprintf(buffer, "%2dA", metrics.amps*2);
//    sprintf(buffer, "%3d", config.holdThrottle);
    u8x8.drawString(0, 2, buffer);
    sprintf(buffer, "%5d", metrics.rpm);
    u8x8.drawString(0, 3, buffer);

#else
    SET_FONT_S;
    sprintf(buffer, " %2d %%", metrics.throttlePcnt);
    u8x8.drawString(0, 0, buffer);
    if (!VOLTS_DISABLED) {
        sprintf(buffer, "%2d.%1dV", metrics.volts/10, metrics.volts%10);
        u8x8.drawString(0, 1, buffer);
    }
    if (!CURRENT_DISABLED) {
        sprintf(buffer, "%2d.%1dA", metrics.amps/5, (metrics.amps*2)%10);
        u8x8.drawString(0, 2, buffer);
    }
//    sprintf(buffer, "%2d.%1d ", metrics.amps/5, (metrics.amps*2)%10);
//    u8x8.drawString(0, 3, buffer);

#endif

#endif

#ifdef SCREEN_32X4_OLD
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
        sprintf(buffer, "%2d %1d", metrics.amps/10, metrics.amps%10);
        sprintf(&buffer[10], "A");
    }
    else {
        sprintf(buffer, "     ");
        sprintf(&buffer[10], " ");
    }
    SET_FONT_L;
    u8x8.drawString(0, 2, buffer);

//    u8x8.drawString(5, 0, "10 8");
//    u8x8.drawString(5, 0, "350");
//    u8x8.drawString(5, 2, "78%");

    SET_FONT_S;
    u8x8.drawString(2, 3, &buffer[10]);
//    u8x8.drawString(5, 1, "K");


    // @todo temps
#endif

}
void drawAfterScreen(unsigned char which) {

    int i;

    clearScreen();

#ifdef SCREEN_32X4
    SET_FONT_L;
    // main sum page
    if (which == 1) {

        // flight #
        u8x8.drawString(0, 0, "# ??");

        // flight time and result
        sprintFixedTimeAndResult();

        // time
        u8x8.drawString(0, 2, buffer);

#ifdef PIN_CURRENT
        // pre-print "MAH" since we have font L here (we save a SET_FONT_L later)
        u8x8.drawString(13, 1, "MAH");
#else
        // pre-print "V"
        u8x8.drawString(13, 1, "V");
#endif
        SET_FONT_S;
        // result
        floatBuffer[3] = 0;
        u8x8.drawString(13, 0, floatBuffer);

        if (metricsSum.holdMode == HOLD_MODE_HOLD_THROTTLE) {
//            sprintf(floatBuffer, " %2d %%", metricsSum.holdValueRaw);
            sprintf(floatBuffer, " %2d %%", config.holdThrottle);
        }
        else if (metricsSum.holdMode == HOLD_MODE_SMART_THROTTLE) {
            sprintf(floatBuffer, "%2d-%2d%%", config.holdThrottle, saved.smartEndThrottle);
        }
//        else if (metricsSum.holdMode == HOLD_MODE_POWER) {
//            // power, formula: P = 10*t/c
//            sprintf(floatBuffer, "  %3dW", 0);
//        }
        else {
            sprintf(floatBuffer, "NOTIMPL");
        }
        sprintf(buffer, " %s %s", holdModeLabels[metricsSum.holdMode], floatBuffer);
        u8x8.drawString(4, 3, buffer);

        SET_FONT_XL;
#ifdef PIN_CURRENT
        // consumption mAh
        // 1mAh = 3.6As
        // AVG_AMPS[A] * FLYTIME[s] / 3.6 = ... mAh
        // test with max time 10m. 10m with max 50A (30 is max) is still just 8333MAH so fits
//        i = 6000;
        i = metricsSum.flightTime;
        i = ((long)i) * metricsSum.ampsAvg / 18;
        sprintf(floatBuffer, "%4d", min(9999, i));
#else
        i = metrics.volts;
        sprintf(floatBuffer, "%2d.%1d", i/10, i%10);
#endif
        u8x8.drawString(5, 0, floatBuffer);

    }
    // volts and amps screen
    else if (which == 2) {
//        SET_FONT_L;
        draw3Decimals('V', metricsSum.voltsMin, metricsSum.voltsAvg, metricsSum.voltsMax);
        u8x8.drawString(0, 0, buffer);

#ifdef PIN_CURRENT
        draw3Decimals('A', metricsSum.ampsMin, metricsSum.ampsAvg, metricsSum.ampsMax);
        u8x8.drawString(0, 2, buffer);
#endif
    }
    // power / rpm view
    else if (which == 3) {
//        SET_FONT_L;
#ifdef PIN_RPM
        draw3Decimals('R', metricsSum.rpmMin, metricsSum.rpmAvg, metricsSum.rpmMax);
#else
        sprintf(buffer, "R%15s", comingSoon);
#endif
        u8x8.drawString(0, 0, buffer);

#ifdef PIN_CURRENT
        draw3Decimals('P', metricsSum.pMin, metricsSum.pAvg, metricsSum.pMax);
#else
        sprintf(buffer, "P%15s", comingSoon);
#endif
        u8x8.drawString(0, 2, buffer);

    }
    // T1 / T2 view
    else if (which == 4) {
//        SET_FONT_L;
        u8x8.drawString(0, 0, "T1   COMING SOON");
        u8x8.drawString(0, 2, "T2   COMING SOON");
    }
    // WELLDONE screen OK
    else if (metricsSum.result < RESULT_ERRORS_FROM) {
        SET_FONT_XL;
        u8x8.drawString(0, 0, "WELLDONE");
        SET_FONT_S;
        u8x8.drawString(0, 3, "   @LOS.CIRCULOS");
    }
    // OOPS screen
    else {
//        SET_FONT_L;

        // as per now, I can save 50+ bytes of memory if I print only "OOPS", and log is not even implemented yet

        // @todo write flight number, obtained after saving after-flight metrics
        u8x8.drawString(0, 0, "# ??");

        // flight time and result
        sprintFixedTimeAndResult();
        u8x8.drawString(0, 2, buffer);
        SET_FONT_S;
        u8x8.drawString(6, 3, floatBuffer);

        SET_FONT_XL;
        u8x8.drawString(4, 0, "!OOPS!");

        // @todo I could put cut data on bottom right but it needs coding and memory soooo

    }

#endif
}
