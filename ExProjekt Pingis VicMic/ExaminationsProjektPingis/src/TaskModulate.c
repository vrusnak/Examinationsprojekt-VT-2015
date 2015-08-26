/*
* TaskModulate.c
*
* Created: 2015-08-26
* Author: Viktor Rusnak, Michael Nilsson
*/

#include <asf.h>
#include "TaskModulate.h"
#include "TaskSendToMatlab.h"
#include "FuncADC.h"
#include "FuncPWM.h"
#include "Global.h"
#include "SerialUART.h"

#define WINDOW_SIZE 4

static int ready = 0;

static uint32_t desiredValue = 0;
static uint32_t initialU = 0;
static int32_t finalU = 0;

static int32_t error = 0;
static int32_t prevError = 0;
static int32_t w = 0;

static double kp = 0.3;
static double ki  = 2.5;
static double kd   = 0.375;
static double samplingTime = 0.1;

/////////////////////ali
static uint32_t sampleTimeMilli = 50;

//static uint32_t distanceValues[WINDOW_SIZE] = {0};
//static uint8_t statusControllerReady = 0;
/////////////////////

void taskModulate(void *p)
{
	portTickType xLastWakeTime;
	portTickType xSampleTime;
	xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{
		xSampleTime = (portTickType)sampleTimeMilli;
		vTaskDelayUntil(&xLastWakeTime, xSampleTime);
		
		if(flag == 0)
		{
			ready = 0;
			
			error = 0;
			prevError = 0;
			w = 0;

			CalcFanValue(0);
		}
		
		if(flag == 1 && ready == 0)
		{
			if (isRXReady())
			{
				if((readByte() != 1) || (readByte() != 0))
				{
					SetDesiredValue(readByte());
					ready = 1;
				}
			}
		}
		
		if(flag == 1 && ready == 1)
		{
			int32_t currSensorValue = ReadFanValue();
			error = desiredValue - currSensorValue;
			
			if(error < 0)
			{
				finalU = initialU;
				w=100;
			}
			else
			{
				finalU = CalcSignal(samplingTime, kp, ki, kd, error, prevError, w);
				if(finalU < 0)
				{
					finalU = abs(finalU);
				}
				else if (finalU > 100)
				{
					finalU = 100;
				}
			}
			prevError = error;
			w = w + prevError;
			
			CalcFanValue(finalU);
			setInfo(finalU, currSensorValue, error);
		}
	}
}

int32_t CalcSignal(double sampTime, double k_p, double k_i, double k_d, int32_t currErr, int32_t prevErr, int32_t sumErr)
{
	double proportionalPart;
	double integralPart;
	double derivingPart;
	int32_t signal;
	
	proportionalPart = k_p * (double)currErr;
	integralPart = (double)sumErr * (sampTime/k_i);
	derivingPart = ((double)currErr - (double)prevErr) * (k_d / sampTime);
	signal = proportionalPart + integralPart + derivingPart;
	//printf("Signal %d\n", signal);
	
	return signal;
}


void SetDesiredValue(uint32_t value)
{
	switch(value)
	{
		case 20:
		desiredValue = DISTANCE_20;
		initialU = 85;
		break;
		case 25:
		desiredValue =  DISTANCE_25;
		initialU = 77;
		break;
		case 30:
		desiredValue = DISTANCE_30;
		initialU = 70;
		break;
		case 35:
		desiredValue = DISTANCE_35;
		initialU = 62;
		break;
		case 40:
		desiredValue = DISTANCE_40;
		initialU = 55;
		break;
	}
}

//void setControllerReady(void){
//statusControllerReady = 1;
//}
//
//void stopController(void){
//statusControllerReady = 0;
//previousErrorValue = 0;
//sumOfErrorValues = 0;
//changeDutyCycle(34, 0);
//}
//
//void setParameters(uint8_t newSetpoint,	uint8_t newSampleTime, double newKp, double newKi, double newKd){
//setSetpoint(newSetpoint);
//sampleTimeMilli = newSampleTime;
//sampleTimeSec = (double)newSampleTime / 1000;
//kp = newKp;
//ki = newKi;
//kd = newKd;
//}