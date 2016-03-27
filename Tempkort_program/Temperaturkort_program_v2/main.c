/*
 * Tempkort_program_v1.cpp
 *
 * Created: 09.03.2016 17.37.07
 * Author : SpaceY
 */ 

#define F_CPU 8000000UL
#define FRSYNC_ON_PORT_B

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "max31855.h"

// #define SPI_DDR_CS DDRC
// #define SPI_PORT_CS PORTC
#define SPI_CS1 PC0
#define SPI_CS2 PC1

static volatile unsigned char last_val;

int main(void){
	
	/* Initialize thermocouple interface library */
	max31855_init(&PORTC,&DDRC,(_BV(SPI_CS1) | _BV(SPI_CS2)));
	
	
	
	
	
	while(1){
		
		/* Acquire one sample from sensor IC */
		uint32_t result = max31855_get(&PORTC,_BV(SPI_CS2));
		
		
		
		
		
	}
}


// 		/* Mask out only what we want to send */
// 		uint32_t mask = 0xFFF00000;
// 		uint32_t masked_result = result & mask;
// 		uint16_t send_this = (masked_result >> 20);
