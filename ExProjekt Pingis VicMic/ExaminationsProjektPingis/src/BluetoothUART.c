/*
 * BluetoothUART.c
 *
 * Created: 2015-05-03 14:27:04
 *  Author: Ali
 */ 
#include <asf.h>
#include "BluetoothUART.h"



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
	/* Stdout shall not be buffered */
	#if defined(__GNUC__)
	setbuf(stdout, NULL);
	#else
	/* Redan i fallet IAR's Normal DLIB default
	konfiguration:
	* sänder en tecken åtgången
	*/
	#endif	
}

void sendDoubleAsASCII(double value){
	char str[10] ={0};
	waitTXReady();
	
	sprintf(str, "%0.3f", value);
	printf("%s\n", str);
}

void sendIntAsASCII(int value){
	waitTXReady();
	printf("%d\n", value);
}

void sendByte(uint8_t value){
	waitTXReady();
	uart_write(CONF_UART, value);
}

uint8_t readByte(void){
	uint8_t value;
	uart_read(CONF_UART, &value);
	
	return value;
}

uint32_t isRXReady(void){
	return UART->UART_SR & UART_SR_RXRDY;
}

uint32_t waitTXReady(void){
	while (!(UART->UART_SR & UART_SR_TXRDY));
	return 1;
}

uint32_t waitRXReady(void){
	while (!(UART->UART_SR & UART_SR_RXRDY));
	return 1;
}

double readDouble(void){
	double result = 0.0;
	char str[10] ={0};
	scanf("%s", str);
	result = atof(str);

	return result;
}