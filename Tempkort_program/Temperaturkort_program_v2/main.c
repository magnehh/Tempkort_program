/*
 * Tempkort_program_v1.cpp
 *
 * Created: 09.03.2016 17.37.07
 * Author : SpaceY
 */ 

#define F_CPU 8000000UL	// Debugging only!
#define UART_BAUD_RATE 9600	// Debugging only!

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>	// Debugging only!
#include <stdlib.h>
#include "uart.h"	// Debugging only!
#include "max31855.h"

// #define SPI_DDR_CS DDRC
// #define SPI_PORT_CS PORTC
#define SPI_CS1 PC0
#define SPI_CS2 PC1

int main(void){
	
	/* Initialize serial port (debugging only!) */
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
	
	/* Initialize thermocouple interface library */
	max31855_init(&PORTC,&DDRC,(_BV(SPI_CS1) | _BV(SPI_CS2)));
	
	/* Interrupts required for UART operation (debugging only!) */
	sei();
	
	while(1){
		/* Circular buffer needed for UART string transmission (debugging only!) */
		char buffer[7];
		
		
		/* Acquire one sample from sensor IC */
		uint32_t result = max31855_get(&PORTC,_BV(SPI_CS2));
		
		
		/* Mask out only what we want to send */
		uint32_t mask = 0xFFF00000;
		uint32_t masked_result = result & mask;
		uint16_t send_this = (masked_result >> 20);
		
		
		/* Transmit temperature through UART */
		uart_puts(ltoa(send_this,buffer,10));
		uart_putc('\n');
		uart_putc('\r');
		
		/* Wait, in order not to overrun debug output */
		_delay_ms(100);
	}
}