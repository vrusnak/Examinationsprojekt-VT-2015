/*
* TaskSend.h
*
* Created: 2015-08-26
* Author: Viktor Rusnak, Michael Nilsson
*/


#ifndef TASKSEND_H_
#define TASKSEND_H_

void taskSendToMatlab(void *p);
void setSendReady(void);
void setInfo(int32_t controlSignal, int32_t actualValue, int32_t error);
void stopSending(void);
int32_t findVacantPositionControlSignal(void);
int32_t findVacantPositionActualValue(void);
void ShiftControlSignalBuffer(void);
void ShiftActualValueBuffer(void);

#endif /* TASKSEND_H_ */