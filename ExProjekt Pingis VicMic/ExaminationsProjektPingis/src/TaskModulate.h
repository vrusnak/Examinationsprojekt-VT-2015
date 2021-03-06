/*
* TaskModulate.h
*
* Created: 2015-08-26
* Author: Viktor Rusnak, Michael Nilsson
*/


#ifndef TASKMODULATE_H_
#define TASKMODULATE_H_

void taskModulate(void *p);
void SetDesiredValue(uint32_t);
void setControllerReady(void);
void stopController(void);
void setParameters(uint8_t,	uint8_t, double, double, double);

int32_t CalcSignal(double sampTime, double k_p, double k_i, double k_d, int32_t currErr, int32_t prevErr, int32_t sumErr);

//#define DISTANCE_20 450
//#define DISTANCE_25 400
//#define DISTANCE_30 370
//#define DISTANCE_35 350
//#define DISTANCE_40 340

#define DISTANCE_20 20
#define DISTANCE_25 25
#define DISTANCE_30 30
#define DISTANCE_35 35
#define DISTANCE_40 40

#endif /* TASKMODULATE_H_ */