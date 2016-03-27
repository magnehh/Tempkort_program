/*************************************************************************
Title:    Temperature relay application for NAROM student rocket
Author:   Magne Haneberg <magnehh |at| stud |.| ntnu |.| no>
Software: AVR-GCC 4.x
Hardware: ATmega48, ATmega88, ATmega168, ATmega328 
License:  GNU General Public License 
          
DESCRIPTION:
    This application acquires temperature samples from MAX31855 chips
	and relays it to a rocket controller.
                    
LICENSE:
    Copyright (C) 2016 Magne Haneberg, GNU General Public License Version 3

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
                        
*************************************************************************/

#define F_CPU 8000000UL
//#define FRSYNC_ON_PORT_B

#include <avr/io.h>
#include <util/delay.h>
/*#include <avr/interrupt.h>*/
/*#include <stdlib.h>*/
#include "max31855.h"
#include "parallel.h"

#define SPI_CS_DDR DDRC
#define SPI_CS_PORT PORTC
#define SPI_CS1 PC0
#define SPI_CS2 PC1

int main(void){
	
	/* Allow voltages to stabilize and warm-up time */
	_delay_ms(500);
	
	/* Initialize thermocouple interface library */
	max31855_init(&SPI_CS_PORT,&SPI_CS_DDR,(_BV(SPI_CS1) | _BV(SPI_CS2)));
	
	/* Initialize parallel interface to rocket controller */
	parallel_init();
	
	while(1){
		
		/* Acquire one sample from each sensor IC */
//		uint32_t result_tc_1 = max31855_get(&PORTC,_BV(SPI_CS1));
		uint32_t result_tc_2 = max31855_get(&PORTC,_BV(SPI_CS2));
		
		/* AB AB are sync words for transmitted data */
		uint8_t byteArray[] = {
			0xAB,
			0xAB,
// 			result_tc_1 >> 24,
// 			result_tc_1 >> 16,
// 			result_tc_1 >> 8,
// 			result_tc_1,
			result_tc_2 >> 24,
			result_tc_2 >> 16,
			result_tc_2 >> 8,
			result_tc_2
		};
		
		/* This function writes a given number of bytes to the rocket controller */
		parallel_puts(byteArray,6);
		
	}
}


// 		/* Mask out only what we want to send */
// 		uint32_t mask = 0xFFF00000;
// 		uint32_t masked_result = result & mask;
// 		uint16_t send_this = (masked_result >> 20);
