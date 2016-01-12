/*
* ADCFunctions.c
*
* Created: 2015-08-26
* Author: Viktor Rusnak, Michael Nilsson
*/

#include <asf.h>
#include "FuncADC.h"

/************************************************************************/
/* Configuration of AD-converter. */
/************************************************************************/
void InitADC(void)
{
	/* Configure power management of ADC timer clock */
	pmc_enable_periph_clk(ID_ADC);
	adc_init(ADC, sysclk_get_main_hz(), 1000000, 8); //	adc_init(ADC, sysclk_get_main_hz(), 20000000, 0);
	adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1);
	adc_set_resolution(ADC, ADC_MR_LOWRES_BITS_10);
	adc_enable_channel(ADC, ADC_CHANNEL_10);
	adc_configure_trigger(ADC, ADC_TRIG_SW, 0);
}

/************************************************************************/
/* Returns a value from channel 10 on the analogue input. */
/************************************************************************/
uint32_t ReadFanValue(void)
{
	uint32_t currSensorValue;
	adc_start(ADC);
	currSensorValue = adc_get_channel_value(ADC, ADC_CHANNEL_10);
	//return currSensorValue;
	
	uint32_t distance = 0;

	if (currSensorValue <= 340){
		distance = 35; // <35cm
	}
	else if(currSensorValue > 340 && currSensorValue <= 380){
		distance = 30;//35cm-30cm
	}
	else if(currSensorValue > 380 && currSensorValue <= 420){
		distance = 25;//30cm-25cm
	}
	else if(currSensorValue > 420 && currSensorValue <= 470){
		distance = 20;//25cm-20cm
	}
	else if(currSensorValue > 470 && currSensorValue <= 570){
		distance = 15;//20cm-15cm
	}
	else if(currSensorValue > 570){
		distance = 10;// >10cm
	}
	
	return distance;
}