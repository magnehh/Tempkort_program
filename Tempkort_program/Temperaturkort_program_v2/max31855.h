#ifndef MAX31855_H
#define MAX31855_H

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



/** 
 *  @file
 *  @code #include <max31855.h> @endcode
 * 
 *  @brief MAX31855 SPI library
 *
 *  @author Magne Haneberg <magnehh |at| stud |.| ntnu |.| no>
 *  @copyright (C) 2016 Magne Haneberg, GNU General Public License Version 3
 */



/** @brief  Initializing function
 *  @param  spi_cs_port The port where chip select lines are located
 *  @param  spi_cs_ddr Data direction register of chip select lines
 *	@param	spi_cs_pin_mask Mask of pins to be used as chip select lines
 *	@return	none
 */
void max31855_init(volatile uint8_t *spi_cs_port, volatile uint8_t *spi_cs_ddr, uint8_t spi_cs_pin_mask);



/** @brief  Function to read the entire string of four bytes presented by the MAX31855
 *  @param  spi_cs_port The port where chip select lines are located
 *	@param	spi_cs_pin Pin connected to desired chip select line
 *	@return 32bit word received from MAX31855
 */
uint32_t max31855_get(volatile uint8_t *spi_cs_port, uint8_t spi_cs_pin);



#endif /* MAX31855_H */