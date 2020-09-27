/*
 * sharprog - An AVR programmer for SHARP Pocket Computers
 *
 * Copyright (c) Ralf Ramsauer, 2020
 *
 * Authors:
 *  Ralf Ramsauer <ralf@vmexit.de>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 */

#include <stddef.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "uart.h"

#define UBRR_VAL ((F_OSC+UART_BAUD*8)/(UART_BAUD*16)-1)
#define BAUD_REAL (F_OSC/(16*(UBRR_VAL+1)))
#define BAUD_ERROR ((BAUD_REAL*1000)/UART_BAUD)

/* disable baud check for the moment */
#if 0
#if ((BAUD_ERROR<985) || (BAUD_ERROR>1010))
  #warn BAUD_ERROR
  #error Choose another crystal. Baud error too high.
#endif
#endif

void uart_init()
{
	// Enable receive and transmit
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	// Asynchronous mode, 8N1
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

	UBRR0H = UBRR_VAL >> 8;
	UBRR0L = UBRR_VAL & 0xFF;
}

void uart_putc(const char c)
{
	while ( !(UCSR0A & (1 << UDRE0)) );
	UDR0 = (const unsigned char)c;
}

void uart_puts(const char *str)
{
	do {
		uart_putc(*str);
	} while(*++str);
}
