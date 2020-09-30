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
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <string.h>

#include <protocol.h>

#include "debug.h"
#include "uart.h"
#include "rs232.h"
#include "sharp.h"

#define MAX_OVF_CTR	100

#define EAGAIN		1

/* ~10ms */
static volatile unsigned char ovf_ctr;
ISR(TIMER0_OVF_vect)
{
	ovf_ctr++;
}

static inline void start_timer(void)
{
	ovf_ctr = 0;
	TCNT0 = 0;
	TCCR0B = (1 << CS00) | (1 << CS02);
}

static inline void stop_timer(void)
{
	TCCR0B = 0;
}

void rs232_init(void)
{
	stop_timer();
	TCCR0A = 0;
	TIMSK0 = (1 << TOIE0);
}

static void rs232_send_success(void)
{
	uart_putc(RS232_SUCCESS);
}

static void rs232_send_fail(void)
{
	uart_putc(RS232_FAIL);
}

static int rs232_rcv_byte(void)
{
	start_timer();
	while (!(UCSR0A & (1 << RXC0))) {
		if (ovf_ctr >= MAX_OVF_CTR) {
			stop_timer();
			return -EAGAIN;
		}
	}
	stop_timer();

	return UDR0;
}

static void tap_mode(void)
{
	static struct {
		uint16_t length;
	} tap_ctx;
	int c;

	memset(&tap_ctx, 0, sizeof(tap_ctx));

	/* Determine the length */
	c = rs232_rcv_byte();
	if (c < 0)
		goto out;

	tap_ctx.length |= (c & 0xff) << 0;
	rs232_send_success();

	c = rs232_rcv_byte();
	if (c < 0)
		goto out;

	tap_ctx.length |= (c & 0xff) << 8;
	pr_dbg_("TAP length: %04x", tap_ctx.length);

	c = sharp_start_transmission(20);
	if (c)
		goto out;
	rs232_send_success();
	dbg_("Rdy!");

	while (tap_ctx.length--) {
		c = rs232_rcv_byte();
		if (c < 0)
			goto out;
		c = enqueue_byte(5, c, false);
		if (c)
			goto out;

		rs232_send_success();
	}
	sharp_stop_transmission();
	rs232_send_success();
	return;

out:
	rs232_send_fail();
}

void rs232_loop(void)
{
	int c;

	for (;;) {
		c = rs232_rcv_byte();
		if (c == -EAGAIN)
			continue;

		/* choose the mode */
		switch (c) {
		case 't':
			dbg_("Entering tape mode");
			rs232_send_success();
			tap_mode();
			break;

		case 'p': /* ping */
			uart_putc(RS232_CMD_PING);
			break;

		default:
			dbg_("Unknown mode");
			rs232_send_fail();
			break;
		}
	}
}
