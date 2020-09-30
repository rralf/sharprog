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

#include <avr/io.h>

#include "config.h"

static inline void led(bool on)
{
	if (on)
		LED_PORT |= (1 << LED_BIT);
	else
		LED_PORT &= ~(1 << LED_BIT);
}

static inline void led_toggle(void)
{
	LED_PORT ^= (1 << LED_BIT);
}

static inline void led_init(void)
{
	LED_DDR |= (1 << LED_BIT);
	led(false);
}


