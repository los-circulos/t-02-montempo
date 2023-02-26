#ifndef MONTEMPO_METRICS_H
#define MONTEMPO_METRICS_H

// OK results
#define RESULT_OK_TIME 0
#define RESULT_OK_V 1

// ERR results
#define RESULT_ERRORS_FROM 2
#define RESULT_ERR_BTN 2

// @todo err max time 9:59

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
    // raw throttle
    unsigned char throttlePcnt;
    // multiplied by 10
    unsigned char volts = 0;
#ifdef PIN_CURRENT
    // multiplied by 5
    unsigned char amps = 0;
#endif
    // rpms raw (???)
    unsigned int rpm = 0;
    // ???
    unsigned char t1 = 0;
    unsigned char t2 = 0;
    // watts / 2
    unsigned char p;
    unsigned long lastTime;
};

struct metricsSumT {
    unsigned char throttleMin;
    unsigned char throttleMax;
    unsigned char throttleAvg;

    unsigned char voltsMin;
    unsigned char voltsMax;
    unsigned char voltsAvg;


    unsigned char t1Min;
    unsigned char t1Max;
    unsigned char t1Avg;

    unsigned char t2Min;
    unsigned char t2Max;
    unsigned char t2Avg;

#ifdef PIN_CURRENT
    unsigned char ampsMin;
    unsigned char ampsMax;
    unsigned char ampsAvg;

    unsigned char pMin;
    unsigned char pMax;
    unsigned char pAvg;
#endif

    unsigned int rpmMin;
    unsigned int rpmMax;
    unsigned int rpmAvg;

    // secs
    unsigned int flightTime;
    unsigned char holdMode;
    unsigned char holdValueRaw;
    unsigned char endValueRaw;
    unsigned char softTime;
    unsigned char voltsBefore;
    unsigned char voltsAfter;
    unsigned char result;
};
struct metricsSumCntT {
    unsigned long throttleSum;
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

#define METRICS_FLIGHT_MILLIS (metricsSumCntT.lastMillis - metricsSumCntT.startMillis)

void rpmISR();
//void initMetrics();
void readMetrics();
void resetMetrics();
void readAndSumMetrics();

#endif //MONTEMPO_METRICS_H
