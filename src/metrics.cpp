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
    if (!VOLTS_DISABLED) {
        metrics.volts = analogRead(PIN_VOLT) / INPUT_DIV_VOLT;
    }
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
    /*
    test readings show RPM value not being linear with rpm
     41 1700    8%  1500
     46 2000    10% 1700
     52 2200    12% 1900
     56 24      14% 2100
     61 26      16% 2250
     65 28      18% 2400
     69 29      20% 2500
     73 31      22% 2700
     77 32      24% 2900
     80 34      26% 3000
     84 36      28% 3100
     87 37      30% 3200
     98 38      32% 3600
        43      40% 5000
        52      50% 13000
     */
//    metrics.rpm = i * 60 / saved.poles *1000 / (currentTime - metrics.lastTime);
//    metrics.rpm = (unsigned long)i * 60000 / saved.poles / (currentTime - metrics.lastTime);
    metrics.rpm = (unsigned long)i * 600 / saved.poles * 100 / (currentTime - metrics.lastTime);
//    metrics.rpm = ((unsigned int)i)*100;
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

    metricsSum.throttleMin = 255;

#ifdef PIN_VOLT
    if (!VOLTS_DISABLED) {
        metricsSum.voltsMin = 255;
    }
#endif
#ifdef PIN_CURRENT
    if (!CURRENT_DISABLED) {
        metricsSum.ampsMin = 255;
    }
#endif
#ifdef PIN_RPM
    metricsSum.rpmMin = 255;
#endif
    metricsSum.holdMode = saved.holdMode;
    metricsSum.holdValueRaw = savedInputValue;

    metricsSum.holdMode = saved.holdMode;
    metricsSum.holdValueRaw = config.holdValueRaw;

    memset(&metricsSumCnt, 0, sizeof metricsSumCnt);
    metricsSumCnt.startMillis = currentTime;

}

void readAndSumMetrics() {

    readMetrics();

    metricsSumCnt.lastMillis = currentTime;
    metricsSumCnt.summedSamples++;
    metricsSum.flightTime = (currentTime - metricsSumCnt.startMillis) / 1000;

    metricsSum.throttleMin = min(metricsSum.throttleMin, metrics.throttlePcnt);
    metricsSum.throttleMax = min(metricsSum.throttleMax, metrics.throttlePcnt);
    metricsSumCnt.throttleSum+= metrics.volts;
    metricsSum.throttleAvg = metricsSumCnt.throttleSum / metricsSumCnt.summedSamples;

#ifdef PIN_VOLT
    if (!VOLTS_DISABLED) {
        metricsSum.voltsMin = min(metricsSum.voltsMin, metrics.volts);
        metricsSum.voltsMax = max(metricsSum.voltsMax, metrics.volts);
        metricsSumCnt.voltsSum+= metrics.volts;
        metricsSum.voltsAvg = metricsSumCnt.voltsSum / metricsSumCnt.summedSamples;
    }
#endif

#ifdef PIN_CURRENT
    if (!CURRENT_DISABLED) {
        metricsSum.ampsMin = min(metricsSum.ampsMin, metrics.amps);
        metricsSum.ampsMax = max(metricsSum.ampsMax, metrics.amps);
        metricsSumCnt.ampsSum += metrics.amps;
        metricsSum.ampsAvg = metricsSumCnt.ampsSum / metricsSumCnt.summedSamples;
    }

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
