#include <Arduino.h>
#include "config.h"
#include "hardware.h"
#include "metrics.h"
#include "saved.h"

#define RPM_BASE 8000

configT config;

// 30A * 15V = 450W (when holding power) / 20A * 15V = 300W
//const unsigned int powerValues[] = { 160, 190, 220, 260, 300, 350, 400, 450 };
//const unsigned int powerValues[] = { 120, 140, 160, 180, 200, 220, 240, 260, 280, 300, 330, 360, 390, 420, 450, 480 };
#ifdef ANY_DIP_INPUT
#ifdef PIN_CURRENT
const unsigned int powerValues[] = { 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 260, 270,
                                     280, 290, 300, 310, 320, 330, 340, 350, 370, 390, 410, 430, 450, 470, 480, 500 };
#endif
#endif
// 3:00 default, 370 + 25sec = leaves 25sec of the 7mins to start timer and to land. 0 stands for run until voltage cut
#ifdef DEVMODE
const unsigned int flyTimeValues[] = { 10, 60, 210, 240, 270, 280, 290, 300, 310, 320, 330, 340, 350, 360, 370, 0 };
#else
#ifdef INPUT_DIP8
// 0, 60, 180, 240, 300, 340, 355, 370
const unsigned int flyTimeValues[] = { 180, 60, 240, 300, 340, 355, 370, 0 };
#endif
#ifdef INPUT_DIP9
// 0, 60, 180, 210, 240, 270, 280, 290, 300, 310, 320, 330, 340, 350, 360, 370
const unsigned int flyTimeValues[] = { 180, 60, 210, 240, 270, 280, 290, 300, 310, 320, 330, 340, 350, 360, 370, 0 };
#endif
#endif

unsigned char i;

// abstracted so I could reuse code when viewing logs? Seems it gets inline-optimized meanwhile :D
void setHoldValues(unsigned char rawHoldValue) {
    config.holdValueRaw = rawHoldValue;
    config.holdThrottle = 98 - rawHoldValue * INPUT_HOLD_THROTTLE_MULT;
    config.holdRPM = RPM_BASE + rawHoldValue * INPUT_HOLD_RPM_MULT;
#ifdef PIN_CURRENT
    config.holdPower = powerValues[rawHoldValue * INPUT_HOLD_RPM_MULT];
#endif
}

void initConfig() {

#ifdef ANY_DIP_INPUT
    config.savedInputMode = true;
    for (i=INPUT_DIP_1; i <= INPUT_DIP_LAST; i++) {
        if (digitalRead(i)) {
            config.savedInputMode = false;
        }
    }
#endif

#ifdef PIN_VOLT
#endif

}
void readConfigInput() {

#ifdef INPUT_DIP8
    i = readDips(4);
#endif
#ifdef INPUT_DIP9
    i = readDips(4);
#endif
    setHoldValues(i);

#ifdef INPUT_DIP8
    i = (readDips(7) - i) / 16;
#endif
#ifdef INPUT_DIP9
    i = (readDips(8) - i) / 16;
#endif
    config.timeFly = flyTimeValues[i];

    // @todo screen rotate delay should be read from eprom (?)
    // @todo default screen should be read from eprom

    // @todo this value should be refreshed in countdown and restart countdown if changes
#ifdef INPUT_DIP8
    config.rotateScreens = !digitalRead(INPUT_DIP_8);
#endif
#ifdef INPUT_DIP9
    config.rotateScreens = !digitalRead(INPUT_DIP_9);
#endif

#ifdef PIN_VOLT
    if (!VOLTS_DISABLED) {
        // guess save cell count 1-5, save error codes over 5
        for (i = 1; (i*42 < metrics.volts); i++);

        // VCUT ERR is when it is not obvious the number of cells eg. 16V can be 4S or 5S
        // eg we use a 5S battery and it gets drained to 16V (Vcut = 3.2). If we power up again,
        //  it will probably be well over 16V and we recognize it as 5S. However, if we
        //  power up on eg. 16.2V then it's not sure if that's 4S or 5S
        // it is a VCUT error as raising VCUT helps.
        if ((i < 5) && (i+1) * (30+saved.voltCut) <= metrics.volts) {
            config.cellCount = CONFIG_CELLS_ERR_VCUT;
        }
        else if (i * (30+saved.voltCut) > metrics.volts) {
            config.cellCount = CONFIG_CELLS_ERR_VLOW;
        }
        else {
            config.cellCount = i;
        }
    }
#endif

}
