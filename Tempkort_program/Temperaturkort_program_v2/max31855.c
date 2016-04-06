/*************************************************************************
Title:    MAX31855 SPI library
Author:   Magne Haneberg <magnehh |at| stud |.| ntnu |.| no>
Software: AVR-GCC 4.x
Hardware: ATmega48, ATmega88, ATmega168, ATmega328 
License:  GNU General Public License 
          
DESCRIPTION:
    The entire string of bytes made available by the MAX31855 is read and returned to the caller as a 32bit word.

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
	#define SPI_SPCR SPCR
	#define SPI_SPSR SPSR
	#define SPI_SPDR SPDR
	#define SPI_SPIF SPIF
	#define SPI_SPE SPE
	#define SPI_MSTR MSTR
	#define SPI_DDR DDRB
	#define SPI_PORT PORTB
	#define SPI_SCK PB5
	#define SPI_MISO PB4
	#define SPI_MOSI PB3
	#define SPI_SS PB2
#endif

#include <avr/io.h>		// Device-specific register definitions
#include <util/delay_basic.h>	// Used for settling delays
#include <inttypes.h>	// ??? not used ???

void max31855_init(volatile uint8_t *spi_cs_port, volatile uint8_t *spi_cs_ddr, uint8_t spi_cs_pin_mask){
	/* Set SPI pins as input/output as desired */
	SPI_DDR |= _BV(SPI_SCK) | _BV(SPI_MOSI) | _BV(SPI_SS);	// SCK, MOSI and SS pins are outputs
	SPI_DDR &= ~(_BV(SPI_MISO));	// MISO pin is an input
	SPI_PORT |= _BV(SPI_SS);	// SS pin needs to be high at all times during Master operation. See Atmel datasheet for more details.
	
	/* Set chip select lines as outputs */
	*spi_cs_ddr |= spi_cs_pin_mask;
	
	/* Set chip select lines high */
	*spi_cs_port |= spi_cs_pin_mask;

	/* Enable SPI in hardware */
	SPI_SPCR = _BV(SPI_SPE) | _BV(SPI_MSTR);	// SPI Enable and SPI Master Mode
}


uint32_t max31855_get(volatile uint8_t *spi_cs_port, uint8_t spi_cs_pin){
	/* Set chip select line of selected slave low */
	*spi_cs_port &= ~(spi_cs_pin);
	/* Allow for some settling delay */
	_delay_loop_1(5);	// Execute assembly NOP (no operation) 5 times
	
	/* Write one byte to SPI Data Register to initiate reception */
	SPI_SPDR = 0x00;
	/* Wait while transfer not finished */
	while(!(SPI_SPSR & (1<<SPI_SPIF)));	// Polls SPI Status Register for SPI Interrupt flag indefinitely until flag is set
	/* Read back received byte from SPI data register */
	uint8_t max_part_1 = SPI_SPDR;	// Each part is saved in its own 8bit variable
	
	/* Receive three more bytes */
	SPI_SPDR = 0x00;
	while(!(SPI_SPSR & (1<<SPI_SPIF)));
	uint8_t max_part_2 = SPI_SPDR;

	SPI_SPDR = 0x00;
	while(!(SPI_SPSR & (1<<SPI_SPIF)));
	uint8_t max_part_3 = SPI_SPDR;
	
	SPI_SPDR = 0x00;
	while(!(SPI_SPSR & (1<<SPI_SPIF)));
	uint8_t max_part_4 = SPI_SPDR;
	
	/* Set chip select line of selected slave high */
	*spi_cs_port |= spi_cs_pin;
	
	/* Arrange the four received bytes into a 32bit word */
	uint32_t result = max_part_1;	// Copy the first 8 bits
	result = (result << 8) | max_part_2;	// Shift 8 bits leftward and copy the next 8 bits
	result = (result << 8) | max_part_3;	// Repeat
	result = (result << 8) | max_part_4;	// Repeat
	
	/* Return 32bit word */
	return result;
}