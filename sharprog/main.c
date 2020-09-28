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

#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "../protocol.h"

#define DEVICE	"/dev/ttyUSB0"
#define BAUD	B115200

#define SHARP_TIMEOUT_LONG	(1000 * 10)
#define SHARP_TIMEOUT_SHORT	1000

static const unsigned char test_tap[] = {
  0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5f, 0x14, 0x00, 0x01,
  0x08, 0xde, 0x22, 0x53, 0x45, 0x50, 0x50, 0x22, 0x0d, 0x00, 0x02, 0x08,
  0xde, 0x22, 0x53, 0x45, 0x50, 0x50, 0x22, 0x0d, 0x00, 0x03, 0x08, 0xde,
  0x22, 0x53, 0x45, 0x50, 0x50, 0x22, 0x0d, 0x00, 0x04, 0x08, 0xde, 0x22,
  0x53, 0x45, 0x50, 0x50, 0x22, 0x0d, 0x00, 0x05, 0x08, 0xde, 0x22, 0x53,
  0x45, 0x50, 0x50, 0x22, 0x0d, 0x00, 0x06, 0x08, 0xde, 0x22, 0x53, 0x45,
  0x50, 0x50, 0x22, 0x0d, 0x00, 0x07, 0x08, 0xde, 0x22, 0x53, 0x45, 0x50,
  0x50, 0x22, 0x0d, 0x00, 0x08, 0x08, 0xde, 0x22, 0x53, 0x45, 0x50, 0x50,
  0x22, 0x0d, 0x00, 0x09, 0x08, 0xde, 0x22, 0x53, 0x45, 0x50, 0x50, 0x22,
  0x0d, 0x00, 0x0a, 0x08, 0xde, 0x22, 0x53, 0x45, 0x50, 0x50, 0x22, 0x0d,
  0xff, 0xff, 0x94
};

static int sharp_read_byte(int fd, unsigned int timeout)
{
	struct timeval timeval;
	unsigned char resp;
	fd_set set;
	int b;

	timeval.tv_sec = 0;
	timeval.tv_usec = timeout * 1000;

	FD_ZERO(&set);
	FD_SET(fd, &set);

	b = select(fd + 1, &set, NULL, NULL, &timeval);
	if (b == -1)
		return errno;

	if (b == 0)
		return -ETIME;

	b = read(fd, &resp, sizeof(resp));
	if (b != sizeof(resp)) {
		fprintf(stderr, "Read error: no bytes were read.\n");
		return -EIO;
	}

	return resp;
}

static int sharp_send_byte(int fd, unsigned char byte)
{
	int b;

	b = write(fd, &byte, sizeof(byte));
	if (b != sizeof(byte))
		return -EINVAL;

	return sharp_read_byte(fd, SHARP_TIMEOUT_SHORT);
}

static int sharp_send_byte_response(int fd, unsigned char byte,
				    unsigned char response)
{
	int resp;

	resp = sharp_send_byte(fd, byte);
	if (resp < 0)
		return resp;

	if (resp == response)
		return 0;

	return -EINVAL;
}

static int sharp_ping(int fd)
{
	return sharp_send_byte_response(fd, RS232_CMD_PING, RS232_CMD_PING);
}

static int sharp_open(const char *device)
{
	speed_t baud = BAUD;
	struct termios tty;
	char buffer[16];
	int err, fd;

	fd = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (fd == -1) {
		err = errno;
		perror("open");
		goto close_out;
	}

	memset(&tty, 0, sizeof tty);
	err = tcgetattr (fd, &tty);
	if (err) {
		err = errno;
		perror("tcgetattr");
		goto close_out;
	}

	err = cfsetospeed (&tty, baud);
	if (err) {
		err = errno;
		perror("cfsetospeed");
		goto close_out;
	}

	err = cfsetispeed(&tty, baud);
	if (err) {
		err = errno;
		perror("cfsetispeed");
		goto close_out;
	}

	/* 8 bit chars */
	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
	/* disable break processing */
	tty.c_iflag &= ~IGNBRK;
	/* no signaling chars */
	tty.c_lflag = 0;
	/* no remapping, no delays */
	tty.c_oflag = 0;
	/* read doesn't block */
	tty.c_cc[VMIN]  = 0;
	/* 1s read timeout */
	tty.c_cc[VTIME] = 10;
	/* shutoff xon/xoff ctrl */
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);
	/* ignore modem controls */
	tty.c_cflag |= (CLOCAL | CREAD);
	/* shutoff parity */
	tty.c_cflag &= ~(PARENB | PARODD);
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;

	err = tcsetattr(fd, TCSANOW, &tty);
	if (err) {
		err = errno;
		perror("tcsetattr");
		goto close_out;

	}

	/* clear written bytes of fd before writing */
	do {
		err = read(fd, &buffer, sizeof(buffer));
	} while (err > 0);

	return fd;

close_out:
	close(fd);
	return -err;
}

static int sharp_send_array(int fd, const unsigned char *data, size_t length)
{
	int err = 0;
	size_t i;

	for (i = 0; i < length; i++) {
		err = sharp_send_byte_response(fd, data[i], RS232_SUCCESS);
		if (err)
			break;
	}

	return err;
}

static int sharp_send_short(int fd, uint16_t s)
{
	int err;

	err = sharp_send_byte_response(fd, (s >> 0) & 0xff, RS232_SUCCESS);
	if (err)
		return err;
	err = sharp_send_byte_response(fd, (s >> 8) & 0xff, RS232_SUCCESS);
	if (err)
		return err;

	return err;
}

static int sharp_send_tap(int fd, const unsigned char *tap, size_t length)
{
	int err;

	printf("send cmd\n");
	err = sharp_send_byte_response(fd, RS232_CMD_TAP, RS232_SUCCESS);
	if (err)
		goto out;

	printf("send length\n");
	err = sharp_send_short(fd, length);
	if (err)
		goto out;

	printf("send array\n");
	err = sharp_send_array(fd, tap, length);
	if (err)
		goto out;
	printf("sent. wait to finish.\n");

	err = sharp_read_byte(fd, SHARP_TIMEOUT_LONG);
	if (err < 0) {
		goto out;
	}

	err = err == RS232_SUCCESS ? 0 : -EINVAL;
out:
	return err;
}

int main(void)
{
	int err, fd;

	printf("Hello, world!\n");
	fd = sharp_open(DEVICE);
	if (fd < 0) {
		err = fd;
		goto out;
	}

	printf("Ping programmer...\n");
	err = sharp_ping(fd);
	if (err) {
		errno = -err;
		perror("sharp_ping");
		goto close_fd;
	}
	printf("Success!\n");

	printf("size: %lu\n", sizeof(test_tap));
	err = sharp_send_tap(fd, test_tap, sizeof(test_tap));
	if (err) {
		errno = -err;
		perror("sharp_send_tap");
		goto close_fd;
	}
	printf("Success!\n");

	err = 0;
close_fd:
	close(fd);
out:
	return err;
}
