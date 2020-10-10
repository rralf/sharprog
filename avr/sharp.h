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

int sharp_start_transmission(unsigned short sync);
void sharp_stop_transmission(void);

int enqueue_byte(unsigned char mode, unsigned char byte);
