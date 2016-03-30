/*************************************************************************
Title:    Application-specific library for parallel communication
Author:   Magne Haneberg <magnehh |at| stud |.| ntnu |.| no>
Software: AVR-GCC 4.x
Hardware: ATmega48, ATmega88, ATmega168, ATmega328 
License:  GNU General Public License 
          
DESCRIPTION:
    This library handles the parallel communication to the NAROM student rocket main control board.
	It is intended for use in only this specific application.
                    
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



#if defined(__AVR_ATmega48__) || defined(__AVR_ATmega48A__) || defined(__AVR_ATmega48P__) || defined(__AVR_ATmega48PA__) || defined(__AVR_ATmega48PB__) || \
defined(__AVR_ATmega88__) || defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega88PB__) || \
defined(__AVR_ATmega168__) || defined(__AVR_ATmega168A__)|| defined(__AVR_ATmega168P__)|| defined(__AVR_ATmega168PA__) || defined(__AVR_ATmega168PB__) || \
defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
#if defined(FRSYNC_SPACEY_REV_1)
	#define FRSYNC1_DDR DDRC
	#define FRSYNC1_PORT PORTC
	#define FRSYNC1_PIN PINC
	#define FRSYNC2_DDR DDRB
	#define FRSYNC2_PORT PORTB
	#define FRSYNC2_PIN PINB
	#define FRAMESYNC_1 PC5
	#define FRAMESYNC_2 PB1
	#define D7 PD4
	#define D6 PD3
	#define D5 PD5
	#define D4 PD2
	#define D3 PD6
	#define D2 PD1
	#define D1 PD7
	#define D0 PD0
#endif
#endif

#include <avr/io.h>
#include "parallel.h"



void parallel_init(void){
	FRSYNC1_DDR &= ~(_BV(FRAMESYNC_1));	// Set FRAMESYNC1 pin as input...
	FRSYNC1_PORT &= ~(_BV(FRAMESYNC_1));	// ... without internal pullup
	FRSYNC2_DDR &= ~(_BV(FRAMESYNC_2));	// Set FRAMESYNC2 pin as input...
	FRSYNC2_PORT &= ~(_BV(FRAMESYNC_2));	// ... without internal pullup
	
	DDRD = 0xFF;	// All data pins outputs
	PORTD = 0x00;	// Initial state of data pins 0x00
}

void parallel_putc(uint8_t byte){
	/* The data pins are not mapped linearly, therefore a conversion is required */
	uint8_t data7 = (byte & _BV(7)) >> 3;
	uint8_t data6 = (byte & _BV(6)) >> 3;
	uint8_t data5 = (byte & _BV(5)) >> 0;
	uint8_t data4 = (byte & _BV(4)) >> 2;
	uint8_t data3 = (byte & _BV(3)) << 3;	// Notice: Left shift
	uint8_t data2 = (byte & _BV(2)) >> 1;
	uint8_t data1 = (byte & _BV(1)) << 6;	// Notice: Left shift
	uint8_t data0 = (byte & _BV(0)) >> 0;
	
	uint8_t byteConverted = data7 | data6 | data5 | data4 | data3 | data2 | data1 | data0;	// Combine to one byte
	
	while(!((FRSYNC1_PIN & _BV(FRAMESYNC_1)) | (FRSYNC2_PIN & _BV(FRAMESYNC_2))));	// Wait for logic high on FrameSync lines
	
	PORTD = byteConverted;	// Put data on data lines
	
	while((FRSYNC1_PIN & _BV(FRAMESYNC_1)) | (FRSYNC2_PIN & _BV(FRAMESYNC_2)));	// Wait for logic low on FrameSync lines
}

void parallel_puts(volatile uint8_t data[], uint8_t length){
	for (uint8_t i = 0; i < length; ++i){
		parallel_putc(data[i]);
	}
}