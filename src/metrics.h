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
#define RESULT_ERR_A_OVER 6
#define RESULT_ERR_RPM_READ 7
#define RESULT_ERR_RPM_OVER 8
#define RESULT_ERR_T1_READ 9
#define RESULT_ERR_T2_READ 10


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
//    unsigned int voltsMin;
//    unsigned int voltsMax;
    unsigned char voltsMin;
    unsigned char voltsMax;
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
    int flightTime;
    unsigned char holdMode;
    int holdValue;
    unsigned char result;
};

extern metricsT metrics;
extern metricsSumT metricsSum;

#define METRICS_AVG_VOLTS (metricsSum.voltsSum / metricsSum.summedSamples)
#define METRICS_AVG_AMPS (metricsSum.ampsSum / metricsSum.summedSamples)
#define METRICS_FLIGHT_MILLIS (metricsSum.lastMillis - metricsSum.startMillis)

void rpmISR();
//void initMetrics();
void readMetrics();
void resetMetrics();
void sumMetrics();

#endif //MONTEMPO_METRICS_H
