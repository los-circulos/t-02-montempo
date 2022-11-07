#include "metrics.h"
#include "hardware.h"

metricsT metrics;
metricsSumT metricsSum;

void readMetrics() {
#ifdef PIN_VOLT
    metrics.volts = analogRead(PIN_VOLT) / INPUT_DIV_VOLT;
#endif
#ifdef PIN_CURRENT
    int i;
    i = analogRead(PIN_CURRENT);
////    (BASEV * analogRead(PIN_CURRENT) / 1024 - BASEV/2) / 0.066;
    metrics.amps = (BASEV * i / 512 - BASEV) * 75.76;
#endif
//    metrics.amps = analogRead(PIN_CURRENT);
}

void readAmps() {
//    int i = analogRead(PIN_CURRENT);
//    float jj = (BASEV * analogRead(PIN_CURRENT) / 1024 - BASEV/2) / 0.066;
//    float jj = (BASEV * analogRead(PIN_CURRENT) / 512 - BASEV) / 0.132;
//    float jj = (BASEV * i / 512 - BASEV) * 7.576;
}
