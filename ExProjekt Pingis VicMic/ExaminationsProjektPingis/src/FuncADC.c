/*
* ADCFunctions.c
*
*/

#include <asf.h>
#include "FuncADC.h"

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

uint32_t ReadFanValue(void)
{
	uint32_t currSensorValue;
	adc_start(ADC);
	currSensorValue = adc_get_channel_value(ADC, ADC_CHANNEL_10);
	return currSensorValue;
}