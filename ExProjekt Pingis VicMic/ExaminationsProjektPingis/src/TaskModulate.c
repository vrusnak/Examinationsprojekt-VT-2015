/*
* TaskModulate.c
*
* Created: 2015-01-09
* Author: Viktor Rusnak, Michael Nilsson
*/

#include <asf.h>
#include "TaskModulate.h"
#include "TaskSendToMatlab.h"
#include "FuncADC.h"
#include "FuncPWM.h"
#include "Global.h"
#include "SerialUART.h"

#define POSITIONS 6

/************************************************************************/
/* Declaration of filter. */
/************************************************************************/
static int32_t filter[POSITIONS] = {0};

/************************************************************************/
/* Declaration of a flag for status. */
/************************************************************************/
static int ready = 0;

/************************************************************************/
/* Declaration of signal variables. */
/************************************************************************/
static uint32_t desiredValue = 0;
static int32_t finalU = 0;

/************************************************************************/
/* Declaration of error variables. */
/************************************************************************/
static int32_t error = 0;
static int32_t prevError = 0;
static int32_t w = 0;

/************************************************************************/
/* Declaration of regulation variables. */
/************************************************************************/
//Enligt Z-N
//Period = 2.9s
//static double kp = 0.32;
//static double ki = 1.45;
//static double kd = 0.36;

static double kp = 0.0;
static double ki = 0.0;
static double kd = 0.0;

static double samplingTime = 0.1;
static uint32_t samplingTimeMS = 100;

/************************************************************************/
/* Task that handle the regulation process of the actuator. */
/************************************************************************/
void taskModulate(void *p)
{
	portTickType xLastWakeTime;
	portTickType xSampleTime;
	xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{
		xSampleTime = (portTickType)samplingTimeMS;
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
			/* Send in parameters + desired value. */
			waitRXReady();
			kp = readDouble();
			
			waitRXReady();
			ki = readDouble();
			
			waitRXReady();
			kd = readDouble();

			waitRXReady();
			SetDesiredValue(readByte());
			
			ready = 1;
			
			/* Send in desired value only. */
			//if (isRXReady())
			//{
			//if((readByte() != 1) || (readByte() != 0))
			//{
			//SetDesiredValue(readByte());
			//ready = 1;
			//}
			//}
		}
		
		if(flag == 1 && ready == 1)
		{
			/* Moving average of n positions. */
			filter[POSITIONS-1] = ReadFanValue();
			
			int32_t averageSensorValue = 0;
			
			for(int i = 0; i<POSITIONS; i++)
			{
				averageSensorValue = averageSensorValue + filter[i];
			}
			
			averageSensorValue = averageSensorValue / POSITIONS;
			
			for(int i = 0; i<POSITIONS-1; i++)
			{
				filter[i] = filter[i+1];
			}
			
			filter[POSITIONS-1] = 0;

			/* Calculation of the error. */
			error = desiredValue - averageSensorValue;
			
			/* Due to linearization the sign of the error
			must be changed in order to get a positive signal.*/
			error = error * -1;

			finalU = CalcSignal(samplingTime, kp, ki, kd, error, prevError, w);
			
			if (finalU < 0)
			{
				finalU = 0;
			}
			
			prevError = error;
			w = w + prevError;
			
			CalcFanValue(finalU);
			setInfo(finalU, averageSensorValue, -error);
		}
	}
}

/************************************************************************/
/* Calculation of the controlsignal. */
/************************************************************************/
int32_t CalcSignal(double sampTime, double k_p, double k_i, double k_d, int32_t currErr, int32_t prevErr, int32_t sumErr)
{
	double proportionalPart;
	double integralPart;
	double derivingPart;
	int32_t signal;
	
	proportionalPart = (double)currErr;
	integralPart = (double)sumErr * (sampTime/k_i);
	derivingPart = ((double)currErr - (double)prevErr) * (k_d / sampTime);
	signal = k_p * (proportionalPart + integralPart + derivingPart);
	
	return signal;
}

/************************************************************************/
/* Setup for default values. */
/************************************************************************/
void SetDesiredValue(uint32_t value)
{
	switch(value)
	{
		case 20:
		desiredValue = DISTANCE_20;
		break;
		case 25:
		desiredValue =  DISTANCE_25;
		break;
		case 30:
		desiredValue = DISTANCE_30;
		break;
		case 35:
		desiredValue = DISTANCE_35;
		break;
		case 40:
		desiredValue = DISTANCE_40;
		break;
	}
}