#ifndef MONTEMPO_METRICS_H
#define MONTEMPO_METRICS_H

// OK results
#define RESULT_OK_TIME 0
#define RESULT_OK_V 1

// ERR results
#define RESULT_ERRORS_FROM 2
#define RESULT_ERR_BTN 2

// error pairs - READ when reading is needed for flight but is unavailable, OVER - over max threshold
#define RESULT_ERR_V_READ 3
#define RESULT_ERR_V_OVER 4
#define RESULT_ERR_A_READ 5
#define RESULT_ERR_RPM_READ 6
#define RESULT_ERR_RPM_OVER 7
#define RESULT_ERR_T1_READ 8
#define RESULT_ERR_T2_READ 9


// cut errors, indexed like this, so they map to testModeLables[error-44+2]
#define REULTS_ERRORS_CUTS_FROM 40
#define RESULT_ERR_T1CUT 40
#define RESULT_ERR_T2CUT 41
#define RESULT_ERR_VCUT 42
#define RESULT_ERR_ACUT 43



struct metricsT {
    // multiplied by 10
    unsigned int volts = 0;
    // multiplied by 5
    unsigned int amps = 0;
    // ???
    unsigned int rpm = 0;
//    unsigned int t1;
//    unsigned int t2;
    unsigned long lastTime;
};

struct metricsSumT {
    // volts *10
    unsigned char voltsMin;
    unsigned char voltsMax;
    unsigned char voltsAvg;
    // amps *5
    unsigned char ampsMin;
    unsigned char ampsMax;
    unsigned char ampsAvg;
    // rpms raw
    unsigned int rpmMin;
    unsigned int rpmMax;
    unsigned int rpmAvg;
    // watts / 2
    unsigned char pMin;
    unsigned char pMax;
    unsigned char pAvg;
//    unsigned int t1Min;
//    unsigned int t1Max;
//    unsigned int t2Min;
//    unsigned int t2Max;
    // secs
    unsigned int flightTime;
    unsigned char holdMode;
    unsigned char holdValueRaw;
    unsigned char result;
};
struct metricsSumCntT {
    unsigned long voltsSum;
    unsigned long ampsSum;
    unsigned long rpmSum;
    unsigned long pSum;
    unsigned long summedSamples;
    unsigned long startMillis;
    unsigned long lastMillis;
};

extern metricsT metrics;
extern metricsSumT metricsSum;

//#define METRICS_AVG_VOLTS (metricsSum.voltsSum / metricsSum.summedSamples)
//#define METRICS_AVG_AMPS (metricsSum.ampsSum / metricsSum.summedSamples)
//#define METRICS_FLIGHT_MILLIS (metricsSum.lastMillis - metricsSum.startMillis)
#define METRICS_AVG_VOLTS metricsSum.voltsAvg
#define METRICS_AVG_AMPS metricsSum.ampsAvg
#define METRICS_FLIGHT_MILLIS (metricsSumCntT.lastMillis - metricsSumCntT.startMillis)

void rpmISR();
//void initMetrics();
void readMetrics();
void resetMetrics();
void readAndSumMetrics();

#endif //MONTEMPO_METRICS_H
