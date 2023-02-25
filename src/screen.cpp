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

char *savedInputModeLabels[] = {"MOTOR", "", "T1CUT", "T2CUT", "MODE ", "V CUT", "POLES", "ENDTH", "GOVI ", "ARM  ", "CALIB", "SOFT ", "DELAY", "NOT  ", "CLEAR", "NOT  "};
char savedInputModeUnits[] = "%%CCVA P";
//char *holdModeLabels[] = {"THRO", "SMRT", "RPM ", "PWR "};
char *holdModeLabels[] = {"THRO", "SMRT"};
// @todo more modes - simulate glow with stunt tank, with uniflow tank, and erratic run with hole on tank
//char *holdModeLabels[] = {"THRO", "RPM ", "PWR ", "SMRT", "GLOW", "UNIF", "ERRA"};
//char *resultLabels[] = {"OKTIME", "OKVCUT", "BTN", "V ???", "VHIGH", "A ???", "AHIGH", "R??", "RHIGH", "T1???", "T2???"};
char *resultLabels[] = {"OK T", "OK V", "BTN", "V?", "VHI", "A?", "R?", "RHI", "T1?", "T2?"};
char *comingSoon = "COMING SOON";

#define FMT_TEST_VALUE_COMMON " %2d%c"
#define FMT_TEST_VALUE_VOLTS "3.%1dV"
#define FMT_TEST_4DECIMALS "%4d"
#define FMT_OFF "OFF "

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
        u8x8.drawString(6, 2, buffer);
    }
#ifdef PIN_CURRENT
    else if (saved.holdMode == HOLD_MODE_POWER) {
        sprintf(buffer, "PWR  %3d", config.holdPower);
        u8x8.drawString(0, 2, buffer);
    }
#endif
    else if (saved.holdMode == HOLD_MODE_SMART_THROTTLE) {
        sprintf(buffer, "SMR %2d-%2d", config.holdThrottle, saved.endThrottle);
        u8x8.drawString(6, 2, buffer);
    }
    else {
        sprintf(buffer, "THRO %2d%%", config.holdThrottle);
        u8x8.drawString(6, 2, buffer);
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
    u8x8.drawString(6,0, buffer);

    if (
// not possible without VOLT measuring:
#ifndef PIN_VOLT
        // flytime 0 would fly until Vcut
        (config.timeFly == 0) ||
        // power calculation needs volts (and current)
        (saved.holdMode == HOLD_MODE_POWER) ||
#else
#ifdef PIN_CURRENT
        // power calculation needs current (and volts)
        (saved.holdMode == HOLD_MODE_POWER) ||
#endif
#endif
        (   (saved.holdMode == HOLD_MODE_SMART_THROTTLE) &&
            (
                // smart throttle end value should be bigger than start
                (config.holdThrottle > saved.endThrottle) ||
                // smart throttle cannot calculate until vcut (at least for now)
                (config.timeFly == 0)
            )
        )
    ) {
        config.preflightError = true;
    }

    if (config.preflightError) {
        u8x8.drawString(0, 0, "NO ");
        u8x8.drawString(0, 2, "FLY!");
    }
    else {
        u8x8.drawString(1, 0, "FLY?");
        sprintf(buffer, "%2d:%02d", config.timeFly / 60, config.timeFly % 60);
        u8x8.drawString(0, 2, buffer);
    }

#endif

}
void drawSavedInputScreen() {

    switch (savedInputMode) {
    case SAVED_INPUT_MODE_SPIN:
    case SAVED_INPUT_MODE_T1_CUT:
    case SAVED_INPUT_MODE_T2_CUT:
    case SAVED_INPUT_MODE_END_THROTTLE:
#ifdef PIN_CURRENT
    case SAVED_INPUT_MODE_CURRENT_CUT_OBS:
#endif
    // poles should never be off, but it cannot be set 0 anyway
    case SAVED_INPUT_MODE_POLES:
        sprintf(buffer, savedInputValue>0 ? FMT_TEST_VALUE_COMMON : FMT_OFF, savedInputValue, savedInputModeUnits[savedInputMode]);
        break;
    case SAVED_INPUT_MODE_VOLT_CUT:
        sprintf(buffer, savedInputValue>0 ? FMT_TEST_VALUE_VOLTS : FMT_OFF, savedInputValue);
        sprintf(floatBuffer, saved.voltCut > 0 ? FMT_TEST_VALUE_VOLTS : FMT_OFF, saved.voltCut);
        break;
    case SAVED_INPUT_MODE_MODE:
        strcpy(buffer, holdModeLabels[savedInputValue]);
        strcpy(floatBuffer, holdModeLabels[saved.holdMode]);
        break;
    case SAVED_INPUT_MODE_SOFT_TIME:
        sprintf(buffer, "%1d.%1ds", savedInputValue/10, savedInputValue%10);
        sprintf(floatBuffer, "%1d.%1ds", saved.softTime/10, saved.softTime%10);
        break;
    case SAVED_INPUT_MODE_COUNTDOWN:
        sprintf(buffer, " %2ds", savedInputValue);
        sprintf(floatBuffer, " %2ds", saved.countdown);
        break;
    default:
        sprintf(buffer, "USED");
        sprintf(floatBuffer, "----");
    }

    switch (savedInputMode) {
    case SAVED_INPUT_MODE_SPIN:
        strcpy(floatBuffer, "    ");
        break;
    case SAVED_INPUT_MODE_END_THROTTLE:
        sprintf(floatBuffer, FMT_TEST_4DECIMALS, saved.endThrottle);
        break;
    case SAVED_INPUT_MODE_T1_CUT:
        sprintf(floatBuffer, saved.t1Cut > 0 ? FMT_TEST_VALUE_COMMON : FMT_OFF, saved.t1Cut, savedInputModeUnits[savedInputMode]);
        break;
    case SAVED_INPUT_MODE_T2_CUT:
        sprintf(floatBuffer, saved.t2Cut > 0 ? FMT_TEST_VALUE_COMMON : FMT_OFF, saved.t2Cut, savedInputModeUnits[savedInputMode]);
        break;
#ifdef PIN_CURRENT
    case SAVED_INPUT_MODE_CURRENT_CUT_OBS:
        sprintf(floatBuffer, saved.currentCut > 0 ? FMT_TEST_VALUE_COMMON : FMT_OFF, saved.currentCut, savedInputModeUnits[savedInputMode]);
        break;
#endif
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
#ifdef PIN_CURRENT
    sprintf(buffer,  "RPM %2d.%1d %2d.%1d A", metrics.rpm/1000, (metrics.rpm%1000)/100, metrics.amps / 5, (metrics.amps*2) % 10);
#else
    sprintf(buffer,  "RPM %2d.%1d       ", metrics.rpm/1000, (metrics.rpm%1000)/100);
#endif
    u8x8.drawString(0, 2, buffer);
#endif
}
void drawRunScreen(unsigned int secsRemain) {

#ifdef SCREEN_32X4

    drawRemainingTime(secsRemain);

    // draw progress
    if (config.timeFly > 0) {
        // timeFly/16 = (16-secsRemain)/x
        // x = 64 - secsRemain*64/timeFly -1 (-1 so printing won't overflow on secsRemain=0)
//        secsRemain = 63 - secsRemain*64/config.timeFly;
//        secsRemain = 43 - secsRemain*44/config.timeFly;
        secsRemain = 59 - secsRemain*60/config.timeFly;

        int i = secsRemain % 4;

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

        // for DEBUG only
//        sprintf(buffer, "%3d ", i);
//        u8x8.drawString(0, 2, buffer);
//        delay(5000);

        u8x8.inverse();


        // @todo this if() should be redundant but with soft start (and only with soft start) there's a bug without it
        if (i>1 && i<16) {
//        for (i--; i>1; i--) {
        for (i--; i>0; i--) {
            u8x8.drawString(i, 3, " ");
        }
        }

    }

    u8x8.noInverse();

    SET_FONT_S;
    sprintf(buffer, " %2d %%", metrics.throttlePcnt);
    u8x8.drawString(0, 0, buffer);
    if (!VOLTS_DISABLED) {
        sprintf(buffer, "%2d.%1dV", metrics.volts/10, metrics.volts%10);
        u8x8.drawString(0, 1, buffer);
    }
#ifdef PIN_CURRENT
    if (!CURRENT_DISABLED) {
        sprintf(buffer, "%2d.%1dA", metrics.amps/5, (metrics.amps*2)%10);
        u8x8.drawString(0, 2, buffer);
    }
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
    if (which == AFTER_SCREEN_SUMMARY) {

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
            sprintf(floatBuffer, " %2d %%", config.holdThrottle);
        }
        else if (metricsSum.holdMode == HOLD_MODE_SMART_THROTTLE) {
            sprintf(floatBuffer, "%2d-%2d%%", metricsSum.throttleMin, metricsSum.throttleMax);
        }
#ifdef PIN_CURRENT
        else if (metricsSum.holdMode == HOLD_MODE_POWER) {
            // power, formula: P = 10*t/c
            sprintf(floatBuffer, "  %3dW", 0);
        }
#endif
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
    // volts and rpm screen
    else if (which == AFTER_SCREEN_VOLT_RPM) {
//        SET_FONT_L;

        draw3Decimals('V', metricsSum.voltsMin, metricsSum.voltsAvg, metricsSum.voltsMax);
        u8x8.drawString(0, 0, buffer);

#ifdef PIN_RPMs
        draw3Decimals('R', metricsSum.rpmMin, metricsSum.rpmAvg, metricsSum.rpmMax);
#else
        sprintf(buffer, "R %14s", FMT_OFF);
#endif
        u8x8.drawString(0, 2, buffer);

    }
    // T1 / T2 view
    else if (which == AFTER_SCREEN_TEMP) {
//        SET_FONT_L;
        u8x8.drawString(0, 0, "T1   COMING SOON");
        u8x8.drawString(0, 2, "T2   COMING SOON");
    }

    // amps / power view
#ifdef PIN_CURRENT
    else if (which == AFTER_SCREEN_CURRENT_POWER) {
//        SET_FONT_L;

//        sprintf(buffer, "A %14s", comingSoon);
        draw3Decimals('A', metricsSum.ampsMin*2, metricsSum.ampsAvg*2, metricsSum.ampsMax*2);
        u8x8.drawString(0, 0, buffer);

        draw3Decimals('P', metricsSum.pMin, metricsSum.pAvg, metricsSum.pMax);
        u8x8.drawString(0, 2, buffer);
    }
#endif

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
