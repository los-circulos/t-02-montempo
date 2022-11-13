#include "metrics.h"
#include "hardware.h"
#include "mytime.h"
#include "saved.h"
#include "config.h"

metricsT metrics;
metricsSumT metricsSum;
metricsSumCntT metricsSumCnt;

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
    metricsSum.holdMode = saved.holdMode;
    metricsSum.holdValueRaw = savedInputValue;

    metricsSum.holdMode = saved.holdMode;
    metricsSum.holdValueRaw = config.holdValueRaw;

    memset(&metricsSumCnt, 0, sizeof metricsSumCnt);
    metricsSumCnt.startMillis = currentTime;

}

void readAndSumMetrics() {
//    readMetrics();
    metricsSumCnt.lastMillis = currentTime;
    metricsSumCnt.summedSamples++;
    metricsSum.flightTime = (currentTime - metricsSumCnt.startMillis) / 1000;

#ifdef PIN_VOLT
    metricsSum.voltsMin = min(metricsSum.voltsMin, metrics.volts);
    metricsSum.voltsMax = max(metricsSum.voltsMax, metrics.volts);
    metricsSumCnt.voltsSum+= metrics.volts;
    metricsSum.voltsAvg = metricsSumCnt.voltsSum / metricsSumCnt.summedSamples;
#endif
#ifdef PIN_CURRENT
    metricsSum.ampsMin = min(metricsSum.ampsMin, metrics.amps);
    metricsSum.ampsMax = max(metricsSum.ampsMax, metrics.amps);
    metricsSumCnt.ampsSum+= metrics.amps;
    metricsSum.ampsAvg = metricsSumCnt.ampsSum / metricsSumCnt.summedSamples;

    // power
    #ifdef PIN_VOLT
    // volts/10 * amps/5 / (2 to fit in char)
    int i = (long)metrics.volts * metrics.amps / 100;
    metricsSum.pMin = min(metricsSum.pMin, i);
    metricsSum.pMax = max(metricsSum.pMax, i);
    metricsSumCnt.pSum+= i;
    metricsSum.pAvg = metricsSumCnt.pSum / metricsSumCnt.summedSamples;
    #endif

#endif
#ifdef PIN_RPM
    metricsSum.rpmMin = min(metricsSum.rpmMin, metrics.rpm);
    metricsSum.rpmMax = max(metricsSum.rpmMax, metrics.rpm);
    metricsSumCnt.rpmSum+= metrics.rpm;
    metricsSum.rpmAvg = metricsSumCnt.rpmSum / metricsSumCnt.summedSamples;
#endif

}
