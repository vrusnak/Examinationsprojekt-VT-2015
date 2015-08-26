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

#define DISTANCE_15 560
#define DISTANCE_20 465
#define DISTANCE_25 425
#define DISTANCE_30 415
#define DISTANCE_35 400
#define DISTANCE_40 390
#define DISTANCE_45 380


#endif /* TASKMODULATE_H_ */