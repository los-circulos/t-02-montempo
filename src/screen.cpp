#include "config.h"
#include "screen.h"
#include "time.h"

U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED
int currentScreen = SCREEN_PRE;

// should never be more than 17, but let's play safe
char buffer[20];
char floatBuffer[6];

void initScreen() {
    u8x8.begin();
    u8x8.setPowerSave(0);
}
void clearScreen() {
    u8x8.clear();
}

void drawLogoLock() {
    if (errorBlink()) {
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
    u8x8.setFont(FONT_XL);
    u8x8.drawString(0, 0, "MONTEMPO");
    u8x8.setFont(FONT_S);
    u8x8.drawString(0, 3, "F2B");
    u8x8.drawString(12, 3, "V1.0");
}

void drawScreen(configT config) {

    u8x8.setFont(FONT_XL);

    if (config.testMode) {
        u8x8.drawString(0, 0, "TEST");
    }
    else {
        u8x8.drawString(0, 0, "FLY ");
    }
    u8x8.setFont(FONT_S);

    if (config.holdRPM) {
        dtostrf(config.RPM/1000, 4, 1, floatBuffer);
        sprintf(buffer, "RPM %s", floatBuffer);
        u8x8.drawString(0, 3, buffer);
    }
    else if (config.holdCurrent) {
        u8x8.drawString(0, 3, "NOTYET:(");
    }
    else {
        sprintf(buffer, "THR %2d%% ", config.throttle);
        u8x8.drawString(0, 3, buffer);
    }

    u8x8.setFont(FONT_L);
    float u = 12.456;
    dtostrf(u, 2, 2, floatBuffer);
    sprintf(buffer, "%s V", floatBuffer);
    u8x8.drawString(9,0, buffer);

    sprintf(buffer, "%2d:%02d", config.timeFly / 60, config.timeFly % 60);
    u8x8.drawString(11, 2, buffer);

}
