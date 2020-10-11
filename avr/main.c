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

#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "debug.h"
#include "uart.h"
#include "led.h"
#include "rs232.h"
#include "sharp.h"

int main(void)
{
	led_init();
	sharp_init();
	uart_init();

	dbg_("Hello, this is sharprog!");
	sei();

	rs232_init();
	rs232_loop();
	__builtin_unreachable();
}
