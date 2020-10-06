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
#include <string.h>

#include "config.h"
#include "debug.h"
#include "utils.h"
#include "uart.h"
#include "sharp.h"

#define CYCLES_RX_SYNC			8
#define TIMER1_TARGET_FREQUENCY_HZ	8000
#define TIMER1_OCR1A			(F_OSC / TIMER1_TARGET_FREQUENCY_HZ)

#define CYCLES_TO_MS(x)	((x)) / (float)(F_OSC / 1000)
#define CYCLES_TO_HZ(x)	(float)F_OSC / (float)(x)

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

static struct {
	bool available;
	bool finished;
	unsigned char val;
} sharp_rx_bit;

static struct {
	unsigned char ctr;
	uint16_t avg;
	unsigned char shift;
} edge;

static inline void xout_wait(bool value)
{
	bool pin;

	do
		pin = !!(SHARP_XOUT_PIN & (1 << SHARP_XOUT_BIT));
	while (pin != value);
}

static bool sharp_bit_sync_wait(void)
{
	while (!sharp_rx_bit.available)
		cpu_relax();
	sharp_rx_bit.available = false;
	return sharp_rx_bit.val;
}

static inline void int0_activate(void)
{
	EIFR |= (1 << INTF0); /* clear int flag */
	EIMSK |= (1 << INT0);
}

static inline void int0_deactivate(void)
{
	EIMSK &= ~(1 << INT0);
}

static inline void timer_stop(void)
{
	TCCR1B = 0;
}

static inline void timer_reset(void)
{
	TCNT1 = 0;
}

static inline void timer_stop_reset(void)
{
	timer_stop();
	timer_reset();
}

static inline void timer_on(void)
{
	TIFR1 |= (1 << TOV1) | (1 << OCF1A); /* clear pending interrupts */
	TCCR1B |= (1 << CS10);
}

static inline void timer_int_off(void)
{
	TIMSK1 = 0;
}

static inline void timer_ovf_on(void)
{
	TIMSK1 |= (1 << TOIE1);
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

ISR(INT0_vect)
{
	uint16_t tcnt = TCNT1;
	bool val;

	if (!tcnt) {
		timer_on();
		return;
	}

	timer_reset();
	if (edge.ctr < CYCLES_RX_SYNC) {
		edge.avg += tcnt;
		edge.ctr++;
		return;
	}

	val = !(tcnt >> edge.shift);
	if (sharp_rx_bit.available) {
		uart_putc('E');
		return;
	}

	sharp_rx_bit.available = true;
	sharp_rx_bit.val = val;
}

ISR(TIMER1_OVF_vect)
{
	timer_int_off();
	sharp_rx_bit.finished = true;
	sharp_rx_bit.available = true;
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
	timer_reset();
	timer_on();
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
	/* configure TX */
	SHARP_XIN_DDR |= (1 << SHARP_XIN_BIT);
	xin_low();

	/* configure RX */
	SHARP_XOUT_DDR &= ~(1 << SHARP_XOUT_BIT);
	SHARP_XOUT_PORT |= (1 << SHARP_XOUT_BIT);

	timer_int_off();
	TCCR1A = 0;
	timer_stop();
	TCCR1C = 0;

	/* Output Compare 1A is only used for reading from the Sharp */
	OCR1A = TIMER1_OCR1A;

	EICRA = (1 << ISC00) | (1 << ISC01); /* INT0: rising edge */
}

static void put_hex_nibble(unsigned char c)
{
	if (c < 10)
		uart_putc('0' + c);
	else
		uart_putc('a' - 10 + c);
}

static void emit_bit(bool bit)
{
	static unsigned char ctr;
	static bool start;
	static unsigned char value;

	/* nothing to do if we receive a further stopbit */
	if (!start && bit)
		return;

	/* set the startbit if we recvd the first 0 */
	if (!start && !bit) {
		value = 0;
		ctr = 0;
		start = true;
		return;
	}

	/* here: start == 1 */
	if (ctr < 4) {
		if (bit)
			value |= (1 << ctr);
		ctr++;
		if (ctr == 4) {
			//uart_putc('a' + (value & 0xf));
			put_hex_nibble(value);
			value = 0;
		}
	} else /* else if (ctr >= 4) */ {
		ctr = 0;
		start = false;
	}
}

static void sharp_measure_frequency(void)
{
	unsigned char i;

	timer_int_off();
	timer_stop_reset();
	timer_ovf_on();

	/* settle down */
	for (i = 0; i < 16; i++) {
		xout_wait(1);
		xout_wait(0);
	}
	memset(&edge, 0, sizeof(edge));

	int0_activate();
	while (edge.ctr != CYCLES_RX_SYNC)
		cpu_relax();
	int0_deactivate();
	timer_stop_reset();

	pr_dbg_("Frequency: %.2fHz", CYCLES_TO_HZ(edge.avg / CYCLES_RX_SYNC));
	pr_dbg_("Bit period: %.2fms", CYCLES_TO_MS((8 * (edge.avg / CYCLES_RX_SYNC))));
	/* calculate shift value */
	while ((edge.avg / CYCLES_RX_SYNC) >> edge.shift)
		edge.shift++;
	pr_dbg_("Shift: %u", edge.shift);
}

void sharp_rx_start(void)
{
	uint16_t last;
	unsigned char ctr;

	memset(&sharp_rx_bit, 0, sizeof(sharp_rx_bit));
	sharp_measure_frequency();

	/* let's start and search for the first 0 */
	int0_activate();
	while (sharp_bit_sync_wait());

	/* go go go, here comes the data */
	last = 0;
	ctr = 1; /* we start at the second zero bit */
	do {
		last <<= 1;
		last |= sharp_bit_sync_wait();
		if (sharp_rx_bit.finished)
			break;

		ctr++;
		if (ctr == 4 && (last & 0xf) == 0) {
			ctr = 0;
			emit_bit(0);
		} else if (ctr == 8 && (last & 0xff) == 0xff) {
			ctr = 0;
			emit_bit(1);
		}
	} while (true);

	int0_deactivate();
	timer_stop();
	dbg_("EOL");
}
