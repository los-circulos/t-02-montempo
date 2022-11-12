#ifndef MONTEMPO_METRICS_H
#define MONTEMPO_METRICS_H

struct metricsT {
    // multiplied by 10
    unsigned int volts = 0;
    unsigned int rpm = 0;
    // multiplied by 10
    unsigned int amps = 0;
//    unsigned int t1;
//    unsigned int t2;
    unsigned long lastTime;
};

struct metricsSumT {
    // volts *10
    unsigned int voltsMin;
    unsigned int voltsMax;
    unsigned long voltsSum;
    // amps *5
    unsigned int ampsMin;
    unsigned int ampsMax;
    unsigned long ampsSum;
    unsigned int rpmMin;
    unsigned int rpmMax;
//    unsigned int t1Min;
//    unsigned int t1Max;
//    unsigned int t2Min;
//    unsigned int t2Max;
    unsigned long startMillis;
    unsigned long lastMillis;
    unsigned long summedSamples;
};

extern metricsT metrics;
extern metricsSumT metricsSum;

void rpmISR();
//void initMetrics();
void readMetrics();
void resetMetrics();
void sumMetrics();

#endif //MONTEMPO_METRICS_H
