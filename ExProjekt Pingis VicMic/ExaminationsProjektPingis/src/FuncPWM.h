/*
 * FuncPWM.h
 *
 * Created: 2015-04-23 17:10:18
 *  Author: Ali Majed
 */ 


#ifndef PWMFUNCTION_H_
#define PWMFUNCTION_H_

void InitPWM(void);
void InitPIN40(void);
//void enablePWMPin6(void);
//void enablePWMPin7(void);
//void enablePWMPin8(void);
//void enablePWMPin9(void);
//void enablePWMPin34(void);
//void enablePWMPin36(void);

void CalcFanValue(uint32_t);

#endif /* FuncPWM_H_ */