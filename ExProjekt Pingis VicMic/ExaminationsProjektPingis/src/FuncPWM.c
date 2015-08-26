/*
* FuncPWM.c
*
* Created: 2015-08-26
* Author: Viktor Rusnak, Michael Nilsson
*/
#include <asf.h>
#include "FuncPWM.h"

pwm_channel_t pwmPIN40;

void InitPWM(void){
	pmc_enable_periph_clk(ID_PWM);
	pwm_channel_disable(PWM, PWM_CHANNEL_3);
	
	pwm_clock_t clock_setting = {
		.ul_clka = 1000 * 1000,
		.ul_clkb = 0,
		.ul_mck = sysclk_get_cpu_hz()
	};
	pwm_init(PWM, &clock_setting);
	
	InitPIN40();
}

void InitPIN40(void)
{	
	pwmPIN40.channel = PWM_CHANNEL_3;
	pwmPIN40.ul_prescaler = PWM_CMR_CPRE_CLKA;
	pwmPIN40.ul_duty = 0;
	pwmPIN40.ul_period = 100;
	pwm_channel_init(PWM, &pwmPIN40);
	pio_set_peripheral(PIOC, PIO_PERIPH_B, PIO_PC8B_PWML3);
	pwm_channel_enable(PWM, PWM_CHANNEL_3);
}

void CalcFanValue(uint32_t dutyCycle)
{
	if(dutyCycle <= 100 && dutyCycle >=0)
	{
		pwm_channel_update_duty(PWM, &pwmPIN40, dutyCycle);
	}
}