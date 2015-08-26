/*
 * BluetoothUART.h
 *
 * Created: 2015-05-03 14:27:32
 *  Author: Ali
 */ 


#ifndef BLUETOOTHUART_H_
#define BLUETOOTHUART_H_


void setupUART(void);
void setupSTDIO(void);
uint32_t isRXReady(void);
uint32_t waitTXReady(void);
uint32_t waitRXReady(void);
void sendDoubleAsASCII(double);
void sendIntAsASCII(int);
void sendByte(uint8_t);
uint8_t readByte(void);
double readDouble(void);

#endif /* BLUETOOTHUART_H_ */