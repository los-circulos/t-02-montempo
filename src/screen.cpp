#include "config.h"
#include "hardware.h"
#include "screen.h"
#include "mytime.h"

U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED
int currentScreen = SCREEN_PRE;

// should never be more than 17, but let's play safe
char buffer[20];
char floatBuffer[6];

char *testModeLabels[] = {"TEST RUN", "SET SMART", "T1 CUT", "T2 CUT", "VOLT CUT", "AMP CUT", "SET MODE", "UNUSED"};
char *testSetModeLabels[] = {"THRO ", "RPM  ", "POWER", "SMART"};

#define SET_FONT_XL     u8x8.setFont(FONT_XL)
#define SET_FONT_L     u8x8.setFont(FONT_L)
#define SET_FONT_S     u8x8.setFont(FONT_S)

void setFontXL() {
    u8x8.setFont(FONT_XL);
}
void setFontL() {
    u8x8.setFont(FONT_L);
}
void setFontS() {
    u8x8.setFont(FONT_S);
}

void initScreen() {
    u8x8.begin();
    u8x8.setPowerSave(0);
}
void clearScreen() {
    u8x8.clear();
}

void drawNotImplemented() {
    SET_FONT_XL;
    if (blinkLed(BLINK_FAST)) {
        u8x8.drawString(0, 1, "NOT YET");
    }
    else {
        clearScreen();
    }
}
void drawLogoLock() {
    SET_FONT_S;
    if (blinkLed(BLINK_FAST)) {
        u8x8.drawString(4, 3, " !REL! ");
    }
    else {
        eraseLogoLock();
    }
}

void eraseLogoLock() {
    u8x8.drawString(4, 3, "       ");
}

void drawWelcome() {
    // show current settings
    clearScreen();
    SET_FONT_XL;
    u8x8.drawString(0, 0, "MONTEMPO");
    SET_FONT_S;
    u8x8.drawString(0, 3, "F2B");
    u8x8.drawString(12, 3, "V1.0");
}

void drawScreen(configT config) {

//    SET_FONT_XL;
    SET_FONT_L;

    if (config.testMode) {
        u8x8.drawString(10, 0, "TEST");
    }
    else {
//        u8x8.drawString(0, 0, "FLY     ");
        u8x8.drawString(10, 0, "FLY?");
    }
//    SET_FONT_S;
    SET_FONT_L;

    if (config.holdRPM) {
        // 20744 JO
//        dtostrf(((float) config.RPM)/1000, 4, 1, floatBuffer);
//        sprintf(buffer, "RPM %s", floatBuffer);
        // 20334 JO
//        sprintf(buffer, "RPM %2d.", config.RPM/1000);
//        u8x8.drawString(0, 3, buffer);
//        sprintf(floatBuffer, "%d", (config.RPM/100)%10);
//        u8x8.drawString(7, 3, floatBuffer);
        // 20342
        u8x8.drawString(0, 2, "RPM ");
        sprintf(buffer, "%2d.", config.RPM/1000);
        u8x8.drawString(4, 2, buffer);
        sprintf(buffer, "%d", (config.RPM/100)%10);
        u8x8.drawString(7, 2, buffer);
    }
    else if (config.holdPower) {
        sprintf(buffer, "PWR  %3d", config.power);
        u8x8.drawString(0, 2, buffer);
    }
    else if (config.smartThrottle) {
        sprintf(buffer, "SMRT%% %2d", config.throttle);
        u8x8.drawString(0, 2, buffer);
    }
    // constant throttle
    else {
//        sprintf(buffer, "THR  %2d%%", config.throttle);
        sprintf(buffer, "THR %% %2d", config.throttle);
//        sprintf(buffer, "SMRT %2d%%", config.throttle);
        u8x8.drawString(0, 2, buffer);
    }

    SET_FONT_L;
    float u = 12.456;
    dtostrf(u, 2, 2, floatBuffer);
//    sprintf(buffer, " %s V", floatBuffer);
    sprintf(buffer, "V  %s", floatBuffer);
//    u8x8.drawString(9,0, buffer);
    u8x8.drawString(0,0, buffer);

    sprintf(buffer, "%2d:%02d", config.timeFly / 60, config.timeFly % 60);
//    u8x8.drawString(11, 2, buffer);
    u8x8.drawString(9, 2, buffer);

}

void drawWaitDot(uint8_t x) {
    SET_FONT_S;
    u8x8.drawString(x + 4, 3, ".");
}

void drawFlyConfirmation(bool show) {
    SET_FONT_XL;
    if (show) {
        u8x8.drawString(14, 0, "!");
    }
    else {
        u8x8.drawString(14, 0, " ");
    }
}

void drawRemainingTime(unsigned int secsRemain) {
    setFontXL();
    sprintf(
        buffer,
        "%02d%s%02d",
        secsRemain/60,
        blinkLed(secsRemain < 5 ? BLINK_FAST : BLINK_NORMAL) ? "." : " ",
        secsRemain % 60
    );
    u8x8.drawString(3, 0, buffer);
}

void drawTestScreen() {
    setFontL();
//    u8x8.drawString(0, 0, testModeLabels[testMode]);
    sprintf(buffer, "%8s %d %2d%%", testModeLabels[testMode], testMode, testValue);
    u8x8.drawString(0, 0, buffer);
}
