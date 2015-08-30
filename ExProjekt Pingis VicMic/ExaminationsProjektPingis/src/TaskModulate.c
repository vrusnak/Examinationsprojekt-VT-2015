/*
* TaskModulate.c
*
* Created: 2015-08-30
* Author: Viktor Rusnak, Michael Nilsson
*/

#include <asf.h>
#include "TaskModulate.h"
#include "TaskSendToMatlab.h"
#include "FuncADC.h"
#include "FuncPWM.h"
#include "Global.h"
#include "SerialUART.h"

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
//static double kp = 0.36;
//static double ki = 1.3;
//static double kd = 0.325;
static double kp = 0.72;
static double ki = 2.6;
static double kd = 0.65;

static double samplingTime = 0.05;
static uint32_t samplingTimeMS = 50;

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
			int32_t currSensorValue = 0;
			int32_t SensorValue;
			for(int i = 1; i<11; i++)
			{
				currSensorValue = currSensorValue + ReadFanValue();
			}
			SensorValue = currSensorValue/10;
			currSensorValue = 0;
			
			error = desiredValue - SensorValue;

			finalU = CalcSignal(samplingTime, kp, ki, kd, error, prevError, w);
			
			if (finalU < 0)
			{
				finalU = 0;
			}
			
			if(error < 0)
			{
				w=0;
			}
			
			prevError = error;
			w = w + prevError;
			
			CalcFanValue(finalU);
			setInfo(finalU, SensorValue, error);
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
	
	proportionalPart = k_p * (double)currErr;
	integralPart = (double)sumErr * (sampTime/k_i);
	derivingPart = ((double)currErr - (double)prevErr) * (k_d / sampTime);
	signal = proportionalPart + integralPart + derivingPart;
	
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