/**
* main.c
*
* Created: 2015-08-26 
* Author: Viktor Rusnak, Michael Nilsson
*/

#include <asf.h>
#include "TaskSendToMatlab.h"
#include "TaskModulate.h"
#include "FuncADC.h"
#include "FuncPWM.h"
#include "SerialUART.h"

static void ConfigureConsole(void)
{
	const usart_serial_options_t uart_serial_options = { .baudrate =CONF_UART_BAUDRATE, .paritytype = CONF_UART_PARITY };
	/* Konfigurera konsol UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
	/* Specifiera att stdout inte ska buffras */
	#if defined(__GNUC__)
	setbuf(stdout, NULL);
	#endif
}


int main (void)
{
	sysclk_init();
	board_init();
	ioport_init();
	InitADC();
	ConfigureConsole();
	InitPWM();
	setupUART();
	
	/************************************************************************/
	/* First task that handles the PID-regulation. The task with highest priority. */
	/************************************************************************/
	if (xTaskCreate(taskModulate, (const signed char * const) "taskModulate", 1024, NULL, 2, NULL) != pdPASS){
		printf("Failed to create taskModulate\n");
	}
	/************************************************************************/
	/* Second task that handles the communication to Matlab. The task with lowest priority. */
	/************************************************************************/
	if (xTaskCreate(taskSendToMatlab, (const signed char * const) "taskSendToMatlab", 1024, NULL, 1, NULL) != pdPASS){
		printf("Failed to create taskSendToMatlab\n");
	}
	
	vTaskStartScheduler();
}
