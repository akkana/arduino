/*****************************************************************************
 *  Copyright (C) 2013 Jim Garlick
 *  Written by Jim Garlick <garlick.jim@gmail.com>
 *  All Rights Reserved.
 *
 *  This file is part of pi-ted-envoy.
 *  For details, see <https://github.com/garlick/pi-ted-envoy>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License (as published by the
 *  Free Software Foundation) version 2, dated June 1991.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the IMPLIED WARRANTY OF MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the terms and conditions of the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foundation,
 *  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA or see
 *  <http://www.gnu.org/licenses/>.
 *****************************************************************************/
/* led.c - interface Kozig ZT.SEG8B4A036A-V1.1 I2C LED module to raspberry pi */

/* This version of part has RST pin (old version had ADR pin - not compatible).
 * RST is active-low reset pin.
 * To enter demo mode, hold SDA low during power on/reset.
 * To enter address mode, hold SCL low during power on/reset. 
 * Factory slave address = 0x27.
 * Cut trace to disable built-in 4.7K pullup.
 */

#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>

#include "led.h"

#define REG_CMD			0x01
#define REG_DAT			0x02
#define REG_RESET		0x03
#define REG_SLEEP		0x04
#define REG_VCOMH       0x05
#define REG_STATUS      0x06
#define REG_BUSY        0x07
#define REG_ADDRESS     0x08
#define REG_BRIGHTNESS  0x0a
#define REG_VERSION     0x1f

#define RESET_OLED      0x06

#define SLEEP_ON        0xa5
#define SLEEP_OFF       0xa1

#define STATUS_RUN      0x00
#define STATUS_SLEEP    0x01
#define STATUS_SET_ADDRESS 0x02
#define STATUS_TEST     0x04
#define STATUS_BUSY     0x10


/* decimal point |= 0x80 */
static const uint8_t charset[] = {
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,  /* [0]: 0-9 */
    0x77,0x7C,0x39,0x5E,0x79,0x71,                      /* [10]: A-F */
    0x40,                                               /* [16]: - */
};

static uint8_t
_num (uint8_t n)
{
    return charset [n % 16];
}

static uint8_t
_char (char c)
{
    return (c >= '0' && c <= '9') ? _num (c - '0')
         : (c >= 'a' && c <= 'f') ? _num (c - 'a' + 10)
         : (c >= 'A' && c <= 'F') ? _num (c - 'A' + 10)
         : (c == '-') ? charset [16]
         : 0; /* blank */
}

static void
_write(int fd, uint8_t *buf, int len)
{
    int n;

    n = write (fd, buf, len);
    if (n < 0) {
        perror ("write");
        exit (1);
    }
    if (n < len) {
        fprintf (stderr, "short write\n");
        exit (1);
    }
}

static int
_read(int fd, uint8_t *buf, int len)
{
    int n;

    n = read (fd, buf, len);
    if (n < 0) {
        perror ("read");
        exit (1);
    }
    if (n == 0) {
        fprintf (stderr, "premature EOF on read\n");
        exit (1);
    }
    return n;
}

static void
_led_display (int fd, uint8_t *val)
{
    uint8_t buf[] = { REG_DAT, val[3], val[2], val[1], val[0] };
    _write (fd, buf, sizeof (buf));
}

void
led_brightness_set (int fd, uint8_t val)
{
    uint8_t buf[] = { REG_BRIGHTNESS, val,  0xff, 0 , 0};
    _write (fd, buf, sizeof (buf));
}

void
led_addr_set (int fd, uint8_t newaddr)
{
    uint8_t buf[] = { REG_ADDRESS, newaddr };
    _write (fd, buf, sizeof (buf));
}

void
led_reset (int fd)
{
    uint8_t buf[] = { REG_RESET, RESET_OLED };
    _write (fd, buf, sizeof (buf));
}

/* 1=sleep, 0=wake */
void
led_sleep_set (int fd, int val)
{
    uint8_t buf[] = { REG_SLEEP, val ? SLEEP_ON : SLEEP_OFF, 0, 0, 0 };
    _write (fd, buf, sizeof (buf));
}

uint8_t 
led_status_get (int fd)
{
    uint8_t wbuf[] = { REG_STATUS };
    uint8_t rbuf[1];

    _write (fd, wbuf, sizeof (wbuf));
    if (_read (fd, rbuf, sizeof (rbuf)) != 1) {
        perror ("read status byte");
        exit (1);
    }
    return rbuf[0];
}

void
led_version_print (int fd)
{
    uint8_t wbuf[] = { REG_VERSION };
    uint8_t rbuf[19];
    int n;

    _write (fd, wbuf, sizeof (wbuf));
    n = _read (fd, rbuf, sizeof (rbuf));
    printf ("%.*s\n", n, (char *)rbuf);
}

void
led_test (int fd)
{
    int i;
    uint8_t buf[4];

    for (i = 0; i < 8; i++) {
        memset (buf, 1<<i, 4);
        _led_display (fd, buf);
        usleep (1000*100);
    }
}

static void
_led_puts (int fd, char *s)
{
    uint8_t buf[4];
    char *p = s;
    int i = 0;

    memset (buf, 0, sizeof (buf));
    for (i = 0, p = s; *p != '\0' && i < 4; p++) {
        if (*p == '.' && i == 0)
            buf[i++] = 0x80;
        else if (*p == '.' && i > 0)
            buf[i - 1] |= 0x80;
        else
            buf[i++] = _char(*p);
    }
    _led_display (fd, buf);
}

void
led_printf (int fd, const char *fmt, ...)
{
    char s[10];
    va_list ap;

    va_start (ap, fmt);
    vsnprintf (s, sizeof (s), fmt, ap);
    va_end (ap);
    _led_puts (fd, s);
}

int
led_init(int addr)
{
    const char *devname = "/dev/i2c-1";
    int fd;

    fd = open (devname, O_RDWR);
    if (fd < 0) {
        perror (devname);
        exit (1);
    }
    if (ioctl (fd, I2C_SLAVE, addr) < 0) {
        perror ("ioctl I2C_SLAVE");
        exit (1);
    }
    return fd;
}

void
led_fini(int fd)
{
    close (fd);
}

/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */

