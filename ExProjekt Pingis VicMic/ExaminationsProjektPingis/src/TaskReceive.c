/*
* TaskReceive.c
*
* Created: 2015-05-10 15:03:51
*  Author: Ali
*/
#include <asf.h>
#include "TaskReceive.h"
#include "BluetoothUART.h"
#include "TaskSendToMatlab.h"
#include "FuncPWM.h"
#include "TaskModulate.h"

#define HEADER_READY 100
#define HEADER_STOP 101
#define HEADER_SET_PARAMETERS 102
#define HEADER_CHANGE_DUTY 103

void taskReceive(void *p){
	
	for(;;){
		if (isRXReady()){
			uint8_t header = readByte();
			
			switch(header){
				case HEADER_READY:
				{
					setControllerReady();
					setSendReady();
					break;
				}
				break;
				case HEADER_STOP:
				{
					stopController();
					stopSending();
					break;
				}
				case HEADER_SET_PARAMETERS:
				{
					uint8_t newSetpoint;
					uint8_t newSampleTime;
					double newKp;
					double newKi;
					double newKd;
					waitRXReady();
					newSetpoint = readByte();
					waitRXReady();
					newSampleTime = readByte();
					waitRXReady();
					newKp = readDouble();
					waitRXReady();
					newKi =  readDouble();
					waitRXReady();
					newKd = readDouble();
					
					setParameters(newSetpoint, newSampleTime, newKp, newKi, newKd);
					break;
				}
				case HEADER_CHANGE_DUTY:
				{
					uint8_t duty;
					waitRXReady();
					duty = readByte();
					
					CalcFanValue(duty);
					break;
				}
				
			}
		}

		
	}
}