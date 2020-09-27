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

void sharp_init(void);

void sharp_reset_checksum(void);
void sharp_enqueue_checksum(void);

void enqueue_data(unsigned char mode, const unsigned char *data,
		  unsigned int length, bool checksum);
void enqueue_byte(unsigned char mode, unsigned char byte, bool checksum);
void enqueue_sync(unsigned short length);
