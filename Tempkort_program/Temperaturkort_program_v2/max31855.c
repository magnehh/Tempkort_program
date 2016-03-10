/*
 * max31855.c
 *
 * Created: 10.03.2016 13.39.14
 *  Author: SpaceY
 */ 


#define SCK PB7
#define MISO PB5
#define MOSI PB6
#define CS1 PB4

#include <avr/io.h>


void max31855_init(void){
	DDRB |= _BV(SCK) | _BV(MISO) | _BV(CS1);
	DDRB &= ~(_BV(MOSI));
	SPCR = _BV(SPE) | _BV(MSTR);
	
	PORTB |= _BV(CS1);	// Set Chip Select line high
}


uint32_t max31855_get(void){
	/* Begin read from thermocouple interface IC */
	PORTB &= ~(_BV(CS1));
	
	SPDR = 0x00;
	while(!(SPSR & (1<<SPIF)));
	uint8_t max_part_1 = SPDR;
	
	SPDR = 0x00;
	while(!(SPSR & (1<<SPIF)));
	uint8_t max_part_2 = SPDR;

	SPDR = 0x00;
	while(!(SPSR & (1<<SPIF)));
	uint8_t max_part_3 = SPDR;
	
	SPDR = 0x00;
	while(!(SPSR & (1<<SPIF)));
	uint8_t max_part_4 = SPDR;
	
	PORTB |= _BV(CS1);
	/* End read from thermocouple interface IC */
	
	uint32_t result = max_part_1;
	result = (result << 8) | max_part_2;
	result = (result << 8) | max_part_3;
	result = (result << 8) | max_part_4;
	
	return result;
}