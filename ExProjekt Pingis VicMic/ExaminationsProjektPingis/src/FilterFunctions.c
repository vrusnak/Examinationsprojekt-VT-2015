/*
 * FilterFunctions.c
 *
 * Created: 2015-06-03 17:36:54
 *  Author: Ali
 */ 
#include <asf.h>
#include "FilterFunctins.h"


static uint32_t position = 0;

uint32_t calcuateMovingAvrage(uint32_t elements[], uint32_t newElement, uint32_t size){
	elements[position] = newElement;
	position = (position + 1) % size;
	
	int32_t movingAvrage = 0;
	
	for(uint32_t i = 0; i < size; i++){
		movingAvrage = movingAvrage + elements[i];
	}
	movingAvrage = movingAvrage / size;
	
	return movingAvrage;
}


uint32_t lookupTable(uint32_t actualValue){
	uint32_t distance = 0; 
	
	if (actualValue <= 390){
		distance = 186; // 60-45
	}
	else if (actualValue > 390 && actualValue <= 399){
 		distance = 372;//40
 	}
	else if(actualValue > 400 && actualValue <= 411){
		distance = 465;//35
	}	
	else if(actualValue > 411 && actualValue <= 421){
		distance = 558;//30
	}
	else if(actualValue > 421 && actualValue <= 430){
		distance = 651;//25
	}
	else if(actualValue > 430 && actualValue <= 439){
		distance = 697;//22
	}
	else if(actualValue > 439 && actualValue <= 494){
		distance = 744;//20
	}
	else if(actualValue > 495 && actualValue <= 570){
		distance = 837;//15
	}
	else if(actualValue > 570){
		distance = 930;// <10
	}
 	
	return distance;
}