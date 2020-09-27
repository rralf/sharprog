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
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "debug.h"
#include "uart.h"
#include "led.h"

#define TIMER1_TARGET_FREQUENCY_HZ	8000
#define TIMER1_OCR1A			(F_OSC / TIMER1_TARGET_FREQUENCY_HZ)

#define BIT_MASK(last, first) \
        ((0xffffffff>> (32 - ((last) + 1 - (first)))) << (first))

static const unsigned char bas_test_img[] PROGMEM = {
	0x00, 0x0a, 0x11, 0xde, 0x22, 0x53, 0x43, 0x48, 0x45, 0x49, 0x53, 0x32, 0x20, 0x47, 0x45, 0x48, 0x54, 0x21, 0x22, 0x0d,
};

static const unsigned char name[] PROGMEM = {
	0, 0, 0, 0, 0, 0, /* Name: inc cksum */
};

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

static const struct sharp_byte sharp_sync = {
	.data = -1,
	.bits = sizeof(sharp_sync.data) * 8,
};

const uint16_t pattern[] = {
	0xcccc, /* 4 KHz square patten */
	0xaaaa, /* 8 KHz square pattern */
	0x0000, /* TBD: currently unused, while actually needed */
	0x0001,
};
static uint16_t current; /* currently active pattern */

/* Information on the checksum */
static struct {
	unsigned int ctr;
	uint16_t sum;
} check;

/*
 * The timer will consume sharp bytes. It consumes entry 0, produces square
 * output, and moves entry 1 to entry 0 once it finished.
 */
static volatile struct sharp_byte sharp_buf[2] = {
	[0] = { .bits = -1, },
	[1] = { .bits = -1, },
};

static inline void timer_start(void)
{
	TCNT1 = 0;
	TCCR1B |= (1 << CS10);
}

static inline void timer_stop(void)
{
	TCCR1B &= ~(1 << CS10);
}

/* COMPA will be fired every 1/8000 second */
ISR(TIMER1_COMPA_vect)
{
	static unsigned char bit = 0;
	static unsigned char byte_bit = 0;

	/* FIXME, add sane end condition */
	if (sharp_buf[0].bits == -1) {
		timer_stop();
		cli();
	}

	if (bit >= 16) {
		bit = 0;
		byte_bit++;
		if (byte_bit >= sharp_buf[0].bits) {
			byte_bit = 0;
			sharp_buf[0] = sharp_buf[1];
			sharp_buf[1].bits = -1;
		}

		current = pattern[!!(sharp_buf[0].data & (1UL << byte_bit))];
	}

	if (current & (1 << bit))
		PORTB |= (1 << PB0);
	else
		PORTB &= ~(1 << PB0);
	bit++;
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

static void enqueue_converted(const struct sharp_byte *byte)
{
	/*
	 * If this case is true, we start a new conversation. Enqueue the first
	 * sharp_byte, and enable IRQs.
	 */
	if (sharp_buf[0].bits == -1) {
		sharp_buf[0] = *byte;
		timer_start();
		sei();
		return;
	}

	while (sharp_buf[1].bits != -1);
	sharp_buf[1] = *byte;
}

static void update_cksum(unsigned char byte)
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

static void enqueue_byte(unsigned char mode, unsigned char byte, bool cksum)
{
	struct sharp_byte converted;

	if (cksum)
		update_cksum(byte);

	convert(&converted, mode, byte);
	dump_regular_byte(mode, byte, &converted);
	enqueue_converted(&converted);

	if (cksum && check.ctr == 120) {
		enqueue_byte(mode, check.sum, false);

		check.ctr = 0;
		check.sum = 0;
	}
}

static void enqueue_sync(unsigned short length)
{
	unsigned short i;

	for (i = 0; i < length; i++) {
		dump_sync();
		enqueue_converted(&sharp_sync);
	}
}

static void enqueue_data(unsigned char mode, const unsigned char *data, unsigned int length, bool cksum)
{
	const unsigned char *r = data;
	unsigned char c;

	for (r = data; r < data + length; r++) {
		c = pgm_read_byte(r);
		enqueue_byte(mode, c, cksum);
	}
}

int main(void)
{
	led_init();
	led(false);

	DDRB |= (1 << PB0);

	TCCR1C = 0;
	OCR1A = TIMER1_OCR1A;

	TIMSK1 = (1 << OCIE1A);
	TCCR1B = (1 << WGM12); /* CTC mode */

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

	check.sum = 0;
	check.ctr = 0;

	/* If we would have old cksum, we would count this byte and immediately write the checksum */
	enqueue_byte(5, 0x5f, false);
	enqueue_byte(5, 0x14, false);

	enqueue_data(8, bas_test_img, sizeof(bas_test_img), true);

	/* 2x 0xff */
	enqueue_byte(8, 0xff, true);
	enqueue_byte(8, 0xff, false);

	/* Cksum */
	enqueue_byte(8, check.sum, false);
#ifdef DEBUG
	pr_dbg_("Cksum: %x", check.sum);
#endif

	// FIXME add shutdown routines!

	for (;;);
	return 0;
}
