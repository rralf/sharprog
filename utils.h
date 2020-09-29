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

#define BIT_MASK(last, first) \
        ((0xffffffff>> (32 - ((last) + 1 - (first)))) << (first))

#define ARRAY_SIZE(array)  (sizeof(array) / sizeof((array)[0]))
