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
};

struct metricsSumT {
    unsigned int voltsMin;
    unsigned int voltsMax;
    unsigned long voltsSum;
    unsigned int voltsCnt;
    // *5
    unsigned char ampsMin;
    // *5
    unsigned char ampsMax;
    // *5
    unsigned long ampsSum;
    unsigned long ampsCnt;
//    unsigned int rpmMin;
//    unsigned int rpmMax;
//    unsigned int t1Min;
//    unsigned int t1Max;
//    unsigned int t2Min;
//    unsigned int t2Max;

};

extern metricsT metrics;
extern metricsSumT metricsSum;

void readMetrics();

#endif //MONTEMPO_METRICS_H
