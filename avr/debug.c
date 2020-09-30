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

#include <stdarg.h>
#include <stdio.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#include "debug.h"
#include "uart.h"

#define DEBUG_BUFFER_SIZE	128

const char *pr_bool(bool b)
{
        /* Ensure a static location of the string to save some flash */
        return pr_cond_str(b, "true", "false");
}

void debug_puts_P(const char *str)
{
	unsigned char byte;

	do {
		byte = pgm_read_byte(str++);
		if (byte)
			uart_putc(byte);
	} while(byte);
}

void debug_printk_P(const char *fmt, ...)
{
	char buf[DEBUG_BUFFER_SIZE];

	va_list ap;

	va_start(ap, fmt);

	vsnprintf_P(buf, sizeof(buf), fmt, ap);
	uart_puts(buf);

	va_end(ap);
}
