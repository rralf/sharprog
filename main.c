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
#include "utils.h"
#include "sharp.h"

static const unsigned char bas_test_img[] PROGMEM = {
	0x00, 0x0a, 0x11, 0xde, 0x22, 0x53, 0x43, 0x48, 0x45, 0x49, 0x53, 0x32, 0x20, 0x47, 0x45, 0x48, 0x54, 0x21, 0x22, 0x0d,
};

static const unsigned char name[] PROGMEM = {
	0, 0, 0, 0, 0, 0, /* Name: inc cksum */
};

int main(void)
{
	led_init();
	led(false);

	sharp_init();

	uart_init();
	dbg_("Hello, this is sharprog!");
	sei();

	/* Synchronisation. Length of one sync cycle: 64ms */
	enqueue_sync(20);

	/* Typ: NEW_BAS, Cksum: No */
	enqueue_byte(5, 0x70, false);

	/* What's this? */
	enqueue_byte(5, 0x00, true);

	/* Name, Cksum: Yes */
	enqueue_data(5, name, sizeof(name), true);

	sharp_reset_checksum();

	/* If we would have old cksum, we would count this byte and immediately write the checksum */
	enqueue_byte(5, 0x5f, false);
	enqueue_byte(5, 0x14, false);

	enqueue_data(8, bas_test_img, sizeof(bas_test_img), true);

	/* 2x 0xff */
	enqueue_byte(8, 0xff, true);
	enqueue_byte(8, 0xff, false);

	/* Cksum */
	sharp_enqueue_checksum();

	// FIXME add shutdown routines!

	for (;;);
	return 0;
}
