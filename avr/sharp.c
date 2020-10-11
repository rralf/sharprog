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

#include "config.h"
#include "debug.h"
#include "utils.h"
#include "uart.h"
#include "sharp.h"

#define TIMER1_TARGET_FREQUENCY_HZ	8000
#define TIMER1_OCR1A			(F_OSC / TIMER1_TARGET_FREQUENCY_HZ)

static volatile struct {
	unsigned char buf[SHARP_RING_BUFFER_SIZE];
	unsigned char head;
	unsigned char tail;
	bool stop;
} rbuf;

const uint16_t pattern[] = {
	0xcccc, /* 2 KHz square patten */
	0xaaaa, /* 4 KHz square pattern */
	0x0000, /* TBD: currently unused, while actually needed */
	0x0001,
};

static inline void timer_stop(void)
{
	TCCR1B = 0;
}

static inline void timer_start(void)
{
	TCNT1 = 0;
	TCCR1B |= (1 << CS10);
}

static inline void timer_int_off(void)
{
	TIMSK1 = 0;
}

static inline void timer_oc1a_on(void)
{
	TIMSK1 |= (1 << OCIE1A);
}

static inline void timer_ctc_on(void)
{
	TCCR1B |= (1 << WGM12); /* CTC mode */
}

static inline void xin_high(void)
{
	SHARP_XIN_PORT |= (1 << SHARP_XIN_BIT);
}

static inline void xin_low(void)
{
	SHARP_XIN_PORT &= ~(1 << SHARP_XIN_BIT);
}

static inline unsigned char rbuf_next_pos(unsigned char pos)
{
	return (pos + 1) % ARRAY_SIZE(rbuf.buf);
}

static int rbuf_enq(unsigned char byte)
{
	if (rbuf.stop)
		return -1;

	while (rbuf.tail == rbuf.head);

	rbuf.buf[rbuf.tail] = byte;
	rbuf.tail = rbuf_next_pos(rbuf.tail);
	return 0;
}

static int rbuf_deq(unsigned char *dst)
{
	unsigned char new_head = rbuf_next_pos(rbuf.head);

	if (new_head == rbuf.tail) {
		dbg_("Underrun");
		return -1;
	}
	*dst = rbuf.buf[new_head];
	rbuf.head = new_head;

	return 0;
}

/* COMPA will be fired every 1/8000 second */
ISR(TIMER1_COMPA_vect)
{
	static uint16_t scurrent;
	static uint16_t current; /* currently active pattern */
	static unsigned char bit = 0;
	static unsigned char byte_bit = 0;
	int err;

	if (bit >= 16) {
		bit = 0;
		byte_bit++;
		if (byte_bit >= 10) {
			byte_bit = 0;
			err = rbuf_deq((unsigned char*)&scurrent);
			if (err) {
				rbuf.stop = true;
				timer_stop();
				return;
			}
			scurrent |= BIT_MASK(9, 5); /* Add stopbits */
		}

		current = pattern[!!(scurrent & (1UL << byte_bit))];
	}

	if (current & (1 << bit))
		xin_high();
	else
		xin_low();
	bit++;
}

static int enqueue_sync(unsigned short i)
{
	while (i--) {
		if (rbuf_enq(0x1f))
			return -1;
	}
	return 0;
}

void sharp_tx_stop(void)
{
	while (!rbuf.stop);

	timer_int_off();
	timer_stop();
}

int sharp_tx_start(unsigned short sync)
{
	int err;

	timer_oc1a_on();
	timer_ctc_on();

	rbuf.stop = false;
	rbuf.head = ARRAY_SIZE(rbuf.buf) - 1;
	rbuf.tail = 0;

	err = enqueue_sync(ARRAY_SIZE(rbuf.buf) / 2);
	if (err)
		return -1;
	timer_start();
	return enqueue_sync(sync);
}

int sharp_tx_enqueue_byte(unsigned char byte)
{
	int err;

	err = rbuf_enq(((byte >> 4) & 0xf) << 1);
	if (err)
		return err;

	return rbuf_enq((byte & 0xf) << 1);
}

void sharp_init(void)
{
	SHARP_XIN_DDR |= (1 << SHARP_XIN_BIT);
	xin_low();

	timer_int_off();
	TCCR1A = 0;
	timer_stop();
	TCCR1C = 0;

	/* Output Compare 1A is only used for reading from the Sharp */
	OCR1A = TIMER1_OCR1A;
}
