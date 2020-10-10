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

/*
 * Let's call it a SHARP byte. A SHARP byte consists of 8 bit of information,
 * split in two nibbles. A nibble is surrounded by one start bit (0) and f and
 * s stopbits (1). f is the number of stopbits for the least significant
 * nibble, l is the number of stopbits for the most significant nibble. Those
 * stopbits determine the mode (see struct mode_info), and the converted sharp
 * byte may vary in size. The bits field hold the number valid bits in data.
 */
struct sharp_byte {
	uint32_t data;
	char bits; /* -1 indicates: waiting for next byte */
};

struct mode_info {
	unsigned char first:4;
	unsigned char second:4;
} __attribute__((packed));

/* Information on the checksum */
static struct {
	unsigned int ctr;
	uint16_t sum;
} check;

static volatile struct {
	struct sharp_byte buf[SHARP_RING_BUFFER_SIZE];
	unsigned char head;
	unsigned char tail;
	bool stop;
} rbuf;

static const struct sharp_byte sharp_sync = {
	.data = -1,
	.bits = sizeof(sharp_sync.data) * 8,
};

/* stop bits of first and second nibble */
static const struct mode_info mode_infos[] = {
	{6, 6},
	{5, 6},
	{5, 5},
	{4, 5},
	{1, 6},
	{1, 5},
	{1, 4},
	{1, 3},
	{1, 2},
};

static inline void timer_stop(void)
{
	TCCR1B &= ~(1 << CS10);
}

static inline void timer_start(void)
{
	TCNT1 = 0;
	TCCR1B |= (1 << CS10);
}

const uint16_t pattern[] = {
	0xcccc, /* 4 KHz square patten */
	0xaaaa, /* 8 KHz square pattern */
	0x0000, /* TBD: currently unused, while actually needed */
	0x0001,
};

#ifdef DEBUG
static void dump_sharp_byte(const struct sharp_byte *sharp_byte)
{
	unsigned char i;

	for (i = 0; i < sharp_byte->bits ; i++)
		uart_putc(sharp_byte->data & (1UL << i) ? '1' : '0');
}

static void dump_regular_byte(unsigned char mode, unsigned char byte,
		      const struct sharp_byte *sharp_byte)
{
	static unsigned int ctr = 0;

	pr_dbg("%04x: B: %02x, M: %u -- ", ctr++, byte, mode);
	dump_sharp_byte(sharp_byte);
	dbg_("");
}

static void dump_sync(void)
{
	dbg("SYNC        -- ");
	dump_sharp_byte(&sharp_sync);
	dbg_("");
}
#else
#define dump_regular_byte(x, y, z)
#define dump_sync()
#endif

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

static int rbuf_enq(const struct sharp_byte *byte)
{
	if (rbuf.stop)
		return -1;

	while (rbuf.tail == rbuf.head);

	rbuf.buf[rbuf.tail] = *byte;
	rbuf.tail = rbuf_next_pos(rbuf.tail);
	return 0;
}

static int rbuf_deq(struct sharp_byte *dst)
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
	static struct sharp_byte scurrent;
	static uint16_t current; /* currently active pattern */
	static unsigned char bit = 0;
	static unsigned char byte_bit = 0;
	int err;

	if (bit >= 16) {
		bit = 0;
		byte_bit++;
		if (byte_bit >= scurrent.bits) {
			byte_bit = 0;
			err = rbuf_deq(&scurrent);
			if (err) {
				rbuf.stop = true;
				timer_stop();
				return;
			}
		}

		current = pattern[!!(scurrent.data & (1UL << byte_bit))];
	}

	if (current & (1 << bit))
		xin_high();
	else
		xin_low();
	bit++;
}

static void update_checksum(unsigned char byte)
{
	uint16_t sum ;

	sum = check.sum + ((byte & 0xF0) >> 4) ;
	if (sum > 0xFF) {
		++sum;
		sum &= 0xFF ;
	}
	check.sum = (sum + (byte & 0x0F)) & 0xFF ;

	check.ctr++;
}

static inline uint16_t encode_nibble(unsigned char nibble, unsigned char sb)
{
	uint16_t ret;

	/*
	 * Shift the nibble one to the left: Bit 0 is the startbit with value 0
	 */
	ret = nibble << 1;
	/* Add sb stopbits with value 1 at the end of the nibble */
	ret |= BIT_MASK(5 + sb - 1, 5);

	return ret;
}

static void convert(struct sharp_byte *dst, unsigned char mode,
		    unsigned char c)
{
	const struct mode_info *m = &mode_infos[mode];
	unsigned char n0 = (c >> 0) & 0xf;
	unsigned char n1 = (c >> 4) & 0xf;

	dst->bits = 1 + 4 + m->first + 1 + 4 + m->second;

	/*
	 * We reverse the bitorder, so we don't need to reverse the bitorder of
	 * the nibbles.
	 */
	dst->data = encode_nibble(n1, m->first);
	dst->data |= encode_nibble(n0, m->second) << (1 + 4 + m->first);
}

void sharp_stop_transmission(void)
{
	while (!rbuf.stop);
}

static int enqueue_sync(unsigned short i)
{
	while (i--) {
		dump_sync();
		if (rbuf_enq(&sharp_sync))
			return -1;
	}
	return 0;
}

int sharp_start_transmission(unsigned short sync)
{
	int err;

	rbuf.stop = false;
	rbuf.head = ARRAY_SIZE(rbuf.buf) - 1;
	rbuf.tail = 0;

	err = enqueue_sync(4);
	if (err)
		return -1;
	timer_start();
	return enqueue_sync(sync);
}

int enqueue_byte(unsigned char mode, unsigned char byte, bool checksum)
{
	struct sharp_byte converted;
	int err = 0;

	if (checksum)
		update_checksum(byte);

	convert(&converted, mode, byte);
	dump_regular_byte(mode, byte, &converted);
	if (rbuf_enq(&converted))
		return -1;

	if (checksum && check.ctr == 120) {
		err = enqueue_byte(mode, check.sum, false);
		check.ctr = 0;
		check.sum = 0;
	}
	return err;
}

void sharp_init(void)
{
	SHARP_XIN_DDR |= (1 << SHARP_XIN_BIT);
	xin_low();

	TCCR1C = 0;
	OCR1A = TIMER1_OCR1A;

	TIMSK1 = (1 << OCIE1A);
	TCCR1B = (1 << WGM12); /* CTC mode */
}
