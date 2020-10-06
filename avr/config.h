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

#ifndef CONFIG_H
#define CONFIG_H

#include <avr/io.h>

#define LED_DDR		DDRB
#define LED_PORT	PORTB
#define LED_BIT		PB5

#define SHARP_XIN_DDR	DDRB
#define SHARP_XIN_PORT	PORTB
#define SHARP_XIN_BIT	PB0

#define SHARP_XOUT_DDR	DDRD
#define SHARP_XOUT_PORT	PORTD
#define SHARP_XOUT_PIN	PIND
#define SHARP_XOUT_BIT	PD2

/* must be a power of two! */
#define SHARP_RING_BUFFER_SIZE	16

#endif
