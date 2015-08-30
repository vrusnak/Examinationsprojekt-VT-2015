/*
* TaskSend.c
*
* Created: 2015-08-26
* Author: Viktor Rusnak, Michael Nilsson
*/

#include <asf.h>
#include "TaskSendToMatlab.h"
#include "SerialUART.h"
#include "FuncADC.h"
#include "FuncPWM.h"
#include "Global.h"

#define SIZE 20

static int32_t circBuffertControlSignal[SIZE] = {0};
static int32_t circBuffertActualValue[SIZE] = {0};
static int32_t circBuffertError[SIZE] = {0};

uint8_t flag = 0;

/************************************************************************/
/* Task that handle the communication to an from a serial port. */
/************************************************************************/
void taskSendToMatlab(void *p)
{
	portTickType xLastWakeTime;
	portTickType xSampleTime;
	
	xSampleTime = 50;
	xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{
		vTaskDelayUntil(&xLastWakeTime, xSampleTime);
		
		if (isRXReady())
		{
			if(readByte() == 1 || readByte() == 0)
			{
				flag = readByte();
			}
		}
		
		if(flag == 1)
		{
			printf("%d\n", circBuffertControlSignal[0]);
			printf("%d\n", circBuffertActualValue[0]);
			printf("%d\n", circBuffertError[0]);
			ShiftControlSignalBuffer();
			ShiftActualValueBuffer();
		}
	}
}

/************************************************************************/
/* Set values for the controlsignal, sensor and the current error. */
/************************************************************************/
void setInfo(int32_t controlSignal, int32_t actualValue, int32_t error)
{
	if(controlSignal>100)
	{
		controlSignal=100;
	}
	circBuffertControlSignal[0] = controlSignal;
	circBuffertActualValue[findVacantPositionActualValue()] = actualValue;
	circBuffertError[0] = error;
}

/************************************************************************/
/* Return the first vacant position in the array. */
/************************************************************************/
int32_t findVacantPositionControlSignal(void)
{
	for(int i = 0; i<SIZE; i++)
	{
		if(circBuffertControlSignal[i] == 0)
		{
			return i;
		}
		if(circBuffertControlSignal[SIZE] != 0)
		{
			ShiftControlSignalBuffer();
			return SIZE;
		}
	}
	return 0;
}

/************************************************************************/
/* Return the first vacant position in the array. */
/************************************************************************/
int32_t findVacantPositionActualValue(void)
{
	for(int i = 0; i<SIZE; i++)
	{
		if(circBuffertActualValue[i] == 0)
		{
			return i;
		}
		if(circBuffertActualValue[SIZE] != 0)
		{
			ShiftActualValueBuffer();
			return SIZE;
		}
	}
	return 0;
}

/************************************************************************/
/* Shift the array down by one step. */
/************************************************************************/
void ShiftControlSignalBuffer(void)
{
	for(int i = 0; i<SIZE-1; i++)
	{
		circBuffertControlSignal[i] = circBuffertControlSignal[i+1];
	}
}

/************************************************************************/
/* Shift the array down by one step. */
/************************************************************************/
void ShiftActualValueBuffer(void)
{
	for(int i = 0; i<SIZE-1; i++)
	{
		circBuffertActualValue[i] = circBuffertActualValue[i+1];
	}
}
