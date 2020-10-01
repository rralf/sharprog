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
HOST_TARGET = sharprog/sharprog

# MCU settings
TARGET_MCU = atmega328p
F_OSC = 16000000
UART_BAUD ?= 115200

# avrdude settings
AVRDUDE_MCU = m328p
PROGRAMMER=usbasp
PORT=usb

# Qemu settings
QEMU_MCU = atmega2560
QEMU_MACHINE = mega2560

# Compiler settings
AVR_PREFIX = avr-
CC = gcc

AVRDUDE_OPTS = $(AVRDUDE) -p $(AVRDUDE_MCU) -c $(PROGRAMMER) -P $(PORT)

AVR_OBJS := avr/main.o avr/uart.o avr/sharp.o avr/rs232.o
ifdef DEBUG
AVR_OBJS += avr/debug.o
endif
QEMU_AVR_OBJS = $(AVR_OBJS:.o=_qemu.o)

HOST_OBJS := sharprog/main.o

AVR_CC = $(AVR_PREFIX)$(CC)
AVR_OBJCOPY = $(AVR_PREFIX)objcopy
AVR_SIZE = $(AVR_PREFIX)size
AVRDUDE = avrdude
QEMU = qemu-system-avr
QEMU_FLAGS = --machine $(QEMU_MACHINE) -serial stdio

# Global {C,LD}FLAGS, for AVR and Host
CFLAGS := -fdata-sections -ffunction-sections
CFLAGS += -Wall -Wextra -Wstrict-prototypes -Wmissing-declarations
CFLAGS += -Iinclude/
LDFLAGS := -Wl,--gc-sections

# Global debug/release specific CFLAGS
ifdef DEBUG
CFLAGS += -DDEBUG -g -O0
else
CFLAGS += -O2
endif

AVR_CFLAGS := $(CFLAGS) -DF_OSC=$(F_OSC) -DF_CPU=F_OSC -DUART_BAUD=$(UART_BAUD)UL
TARGET_CFLAGS := $(AVR_CFLAGS) -mmcu=$(TARGET_MCU)
QEMU_CFLAGS := $(AVR_CFLAGS) -mmcu=$(QEMU_MCU)

TARGET_ELF = avr/$(TARGET).elf
TARGET_HEX = avr/$(TARGET).hex
QEMU_ELF = avr/$(TARGET)_qemu.elf

all: $(HOST_TARGET) $(TARGET_HEX)

$(HOST_TARGET): $(HOST_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(TARGET_ELF): $(AVR_OBJS)
	$(AVR_CC) $(TARGET_CFLAGS) $(LDFLAGS) -o $@ $^

%.hex: %.elf
	$(AVR_SIZE) $^
	$(AVR_OBJCOPY) -O ihex -R .eeprom $^ $@

avr/%.o: avr/%.c
	$(AVR_CC) -c $(TARGET_CFLAGS) -o $@ $^

avr/%_qemu.o: avr/%.c
	$(AVR_CC) -c $(QEMU_CFLAGS) -o $@ $^

$(QEMU_ELF): $(QEMU_AVR_OBJS)
	$(AVR_CC) $(QEMU_CFLAGS) $(LDFLAGS) -o $@ $^

qemu: $(QEMU_ELF)
	$(QEMU) $(QEMU_FLAGS) --bios $^

program: $(TARGET_HEX)
	$(AVRDUDE_OPTS) -U flash:w:$^

reset:
	$(AVRDUDE_OPTS)

# FIXME align with m328p
#fuse:
#	$(AVRDUDE_OPTS) -U lfuse:w:0xfd:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

rfuse:
	$(AVRDUDE_OPTS) -U lfuse:r:-:b -U hfuse:r:-:b -U efuse:r:-:b

clean:
	rm -f $(HOST_OBJS) $(HOST_TARGET)
	rm -f $(AVR_OBJS) $(TARGET_ELF) $(TARGET_HEX)
	rm -f $(QEMU_AVR_OBJS) $(QEMU_ELF)
