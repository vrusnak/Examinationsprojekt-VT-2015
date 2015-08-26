/*
 * SerialUART.c
 *
 * Created: 2015-08-26
 * Author: Viktor Rusnak, Michael Nilsson
 */ 
#include <asf.h>
#include "SerialUART.h"

void setupUART(void){
	pio_configure(PINS_UART_PIO, PINS_UART_TYPE, PINS_UART_MASK, PIO_DEFAULT);
	pmc_enable_periph_clk(ID_UART);
	const sam_uart_opt_t uart0_settings = { sysclk_get_cpu_hz(), CONF_UART_BAUDRATE, UART_MR_PAR_NO };
	ioport_set_pin_mode(PIO_PA8_IDX, IOPORT_MODE_PULLUP);
	uart_init(UART, &uart0_settings);	
}

void setupSTDIO(void){
	const usart_serial_options_t uart_serial_options = { .baudrate = CONF_UART_BAUDRATE, .paritytype = CONF_UART_PARITY };
	stdio_serial_init(CONF_UART, &uart_serial_options);	
	#if defined(__GNUC__)
	setbuf(stdout, NULL);
	#else
	#endif	
}

uint8_t readByte(void){
	uint8_t value;
	uart_read(CONF_UART, &value);
	return value;
}

uint32_t isRXReady(void){
	return UART->UART_SR & UART_SR_RXRDY;
}