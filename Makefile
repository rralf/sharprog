# Sharprog - An AVR programmer for SHARP Pocket Computers
#
# Copyright (c) Ralf Ramsauer, 2020
#
# Authors:
#  Ralf Ramsauer <ralf@vmexit.de>
#
# This work is licensed under the terms of the GNU GPL, version 2.  See
# the COPYING file in the top-level directory.

TARGET = sharprog

# MCU settings
MCU ?= atmega328p
F_OSC ?= 16000000
UART_BAUD ?= 115200

# avrdude settings
AVRDUDE_MCU ?= m328p
PROGRAMMER=usbasp
PORT=usb

# Qemu settings
MCU_QEMU = atmega2560
QEMU_MACHINE = mega2560

AVRDUDE_OPTS = $(AVRDUDE) -p $(AVRDUDE_MCU) -c $(PROGRAMMER) -P $(PORT)

OBJS = main.o uart.o sharp.o
ifdef DEBUG
OBJS += debug.o
endif
OBJS_QEMU = $(OBJS:.o=_qemu.o)

CC = avr-gcc
OBJCOPY = avr-objcopy
SIZE = avr-size
AVRDUDE = avrdude
QEMU = qemu-system-avr
QEMU_FLAGS = --machine $(QEMU_MACHINE) -serial stdio

CFLAGS := -g -O2
CFLAGS += -fdata-sections -ffunction-sections
CFLAGS += -Wall -Wextra -Wstrict-prototypes -Wmissing-declarations
CFLAGS += -DF_OSC=$(F_OSC) -DF_CPU=F_OSC -DUART_BAUD=$(UART_BAUD)UL
ifdef DEBUG
CFLAGS += -DDEBUG
endif

LDFLAGS := -Wl,--gc-sections

CFLAGS_TARGET := $(CFLAGS) -mmcu=$(MCU) -DMCU=$(MCU)
CFLAGS_QEMU := $(CFLAGS) -mmcu=$(MCU_QEMU) -DMCU=$(MCU_QEMU)
TARGET_QEMU = $(TARGET)_qemu

all: $(TARGET).hex

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS_TARGET) $(LDFLAGS) -o $@ $^

%.hex: %.elf
	$(SIZE) $^
	$(OBJCOPY) -O ihex -R .eeprom $^ $@

%.o: %.c
	$(CC) -c $(CFLAGS_TARGET) -o $@ $^

%_qemu.o: %.c
	$(CC) -c $(CFLAGS_QEMU) -o $@ $^

$(TARGET_QEMU).elf: $(OBJS_QEMU)
	$(CC) $(CFLAGS_QEMU) -o $@ $^

qemu: $(TARGET)_qemu.elf
	$(QEMU) $(QEMU_FLAGS) --bios $^

program: $(TARGET).hex
	$(AVRDUDE_OPTS) -U flash:w:$^

reset:
	$(AVRDUDE_OPTS)

# FIXME align with m328p
#fuse:
#	$(AVRDUDE_OPTS) -U lfuse:w:0xfd:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

rfuse:
	$(AVRDUDE_OPTS) -U lfuse:r:-:b -U hfuse:r:-:b -U efuse:r:-:b

clean:
	rm -f $(OBJS) $(OBJS_QEMU)
	rm -f $(TARGET).elf $(TARGET_QEMU).elf $(TARGET).hex
