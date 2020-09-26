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

#include <stdbool.h>

#include <avr/pgmspace.h>

#define D_NL	"\r\n"

#define dbg_(str) dbg(str D_NL)
#define pr_dbg_(str, ...) pr_dbg(str D_NL, ##__VA_ARGS__)

#define pr_cond_str(__cond, __then, __else) \
	!!(__cond) ? PSTR(__then) : PSTR(__else)

const char *pr_bool(bool b);

#ifndef dbg_fmt
#define dbg_fmt(__x)	(__x)
#endif

void debug_puts_P(const char *str);
// FIXME debug_puts_P ignores the format attribute in case of PSTRs
void __attribute__((format(printf, 1, 2))) debug_printk_P(const char *fmt, ...);

#define dbg(str) debug_puts_P(PSTR(dbg_fmt(str)))
#define pr_dbg(fmt, ...) debug_printk_P(PSTR(dbg_fmt(fmt)), ##__VA_ARGS__)
