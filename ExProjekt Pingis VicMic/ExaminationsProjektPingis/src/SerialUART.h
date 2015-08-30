/*
* SerialUART.h
*
* Created: 2015-08-26
* Author: Viktor Rusnak, Michael Nilsson
*/

#ifndef SerialUART_H_
#define SerialUART_H_

void setupUART(void);
void setupSTDIO(void);
uint8_t readByte(void);
uint32_t isRXReady(void);

#endif /* SerialUART_H_ */