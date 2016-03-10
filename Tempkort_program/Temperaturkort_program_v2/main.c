/*
 * Tempkort_program_v1.cpp
 *
 * Created: 09.03.2016 17.37.07
 * Author : SpaceY
 */ 

#define F_CPU 11059200UL
#define UART_BAUD_RATE 57600

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "uart.h"
#include "max31855.h"

int main(void){
	
	uart1_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
	
	max31855_init();
	
	sei();	// Enable interrupts required for UART operation
	
	while(1){
		char buffer[32];	// Needed for UART string transmission
		
		
		/* Acquire one sample from sensor IC */
		uint32_t result = max31855_get();
		
		
		/* Mask out only what we want to send */
		uint32_t mask = 0xFFF00000;
		uint32_t masked_result = result & mask;
		uint16_t send_this = (masked_result >> 20);
		
		
		/* Transmit temperature through UART */
		uart1_puts(ltoa(send_this,buffer,10));
		uart1_putc('\n');
		uart1_putc('\r');
		
		
		_delay_ms(10);
	}
}