#include "metrics.h"
#include "hardware.h"
#include "mytime.h"
#include "saved.h"
#include "config.h"

metricsT metrics;
metricsSumT metricsSum;

#ifdef PIN_RPM
volatile unsigned int rpmCnt = 0;
void rpmISR() {
    rpmCnt++;
}
#endif

void readMetrics() {
    int i;
#ifdef PIN_VOLT
    metrics.volts = analogRead(PIN_VOLT) / INPUT_DIV_VOLT;
#endif
#ifdef PIN_CURRENT
    i = analogRead(PIN_CURRENT);
////    (BASEV * analogRead(PIN_CURRENT) / 1024 - BASEV/2) / 0.066 * 5;
//    metrics.amps = (BASEV * i / 512 - BASEV) * 37.888;
    i = (BASEV * i / 512 - BASEV) * 37.888;
    metrics.amps = i < 0 ? -i : i;
#endif
#ifdef PIN_RPM
    i = rpmCnt;
    rpmCnt = 0;
//    metrics.rpm = i * 60 / saved.poles *1000 / (currentTime - metrics.lastTime);
    metrics.rpm = i * 60000 / saved.poles / (currentTime - metrics.lastTime);
#ifdef RPM_MIN
    if (metrics.rpm < RPM_MIN) {
        metrics.rpm = 0;
    }
#endif
#endif
    metrics.lastTime = currentTime;
}

// call this only from the loop as it needs currentTime (set in each loop but not in setup)
void resetMetrics() {
    // this yields 60bytes less compiled atm than setting each field
    memset(&metricsSum, 0, sizeof metricsSum);
    // this yields much because the added initial values in .h
//    metricsSum = {};
    metricsSum.voltsMin = 255;
    metricsSum.ampsMin = 255;
    metricsSum.rpmMin = 255;
    metricsSum.startMillis = currentTime;
    metricsSum.holdMode = saved.holdMode;
    metricsSum.holdValue = testValue;
}

void sumMetrics() {
#ifdef PIN_VOLT
    metricsSum.voltsMin = min(metricsSum.voltsMin, metrics.volts);
    metricsSum.voltsMax = max(metricsSum.voltsMax, metrics.volts);
    metricsSum.voltsSum+= metrics.volts;
#endif
#ifdef PIN_CURRENT
    metricsSum.ampsMin = min(metricsSum.ampsMin, metrics.amps);
    metricsSum.ampsMax = max(metricsSum.ampsMax, metrics.amps);
    metricsSum.ampsSum+= metrics.amps;
#endif
#ifdef PIN_RPM
    metricsSum.rpmMin = min(metricsSum.rpmMin, metrics.rpm);
    metricsSum.rpmMax = max(metricsSum.rpmMax, metrics.rpm);
#endif

    metricsSum.lastMillis = currentTime;
    metricsSum.summedSamples++;
    metricsSum.flightTime = (currentTime - metricsSum.startMillis) / 1000;

}
