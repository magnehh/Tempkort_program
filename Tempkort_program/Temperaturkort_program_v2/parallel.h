#ifndef PARALLEL_H
#define PARALLEL_H

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



/** 
 *  @file
 *  @code #include <parallel.h> @endcode
 * 
 *  @brief Parallel comm library
 *
 *  @author Magne Haneberg <magnehh |at| stud |.| ntnu |.| no>
 *  @copyright (C) 2016 Magne Haneberg, GNU General Public License Version 3
 */



/** @brief  Initializing function
 *	@return	none
 */
void parallel_init(void);



/** @brief  Function to write one byte to data lines
 *  @param  char Data to be written
 */
void parallel_putc(uint8_t);



/** @brief  Function to write a string of bytes to data lines
 *  @param  string Data to be written
 */
/*void parallel_puts(uint8_t);*/




#endif /* PARALLEL_H */