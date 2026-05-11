/*
 * time function definitions for NOLIBC
 * Copyright (C) 2017-2022 Willy Tarreau <w@1wt.eu>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef _NOLIBC_TIME_H
#define _NOLIBC_TIME_H

#include "std.h"
#include "rars.h"

struct tm {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

static __inline__ __attribute__((always_inline))
uint64_t __nolibc_rars_time_ms(void)
{
	register long _num  __asm__ ("a7") = RARS_Time;
	register long _low  __asm__ ("a0");
	register long _high __asm__ ("a1");

	__asm__ volatile (
		"ecall\n"
		: "=r"(_low), "=r"(_high)
		: "r"(_num)
		: "memory", "cc"
	);

	return ((uint64_t)(uint32_t)_high << 32) | (uint32_t)_low;
}

static __inline__
int __nolibc_is_leap(int year)
{
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

static __inline__
void __nolibc_copy3(char *dst, const char *src)
{
	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[2];
}

static __inline__
void __nolibc_write_2d(char *dst, int value, int pad_space)
{
	int tens = value / 10;
	int ones = value % 10;

	if (pad_space && tens == 0)
		dst[0] = ' ';
	else
		dst[0] = (char)('0' + tens);
	
	dst[1] = (char)('0' + ones);
}

static __inline__
void __nolibc_write_4d(char *dst, int value)
{
	dst[0] = (char)('0' + (value / 1000) % 10);
	dst[1] = (char)('0' + (value / 100) % 10);
	dst[2] = (char)('0' + (value / 10) % 10);
	dst[3] = (char)('0' + value % 10);
}

static __inline__
struct tm *__nolibc_gmtime_utc(time_t t, struct tm *out)
{
	static const int month_days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	uint32_t seconds;
	uint32_t days;
	uint32_t rem;
	int year = 1970;
	int yday;
	int month;
	int mdays;

	if (!out)
		return NULL;
	if (t < 0)
		return NULL;

	seconds = (uint32_t)t;
	days = seconds / 86400u;
	rem = seconds % 86400u;

	out->tm_hour = (int)(rem / 3600u);
	rem %= 3600u;
	out->tm_min = (int)(rem / 60u);
	out->tm_sec = (int)(rem % 60u);

	out->tm_wday = (int)((days + 4u) % 7u);

	while (1) {
		int year_days = __nolibc_is_leap(year) ? 366 : 365;
		if (days < (uint32_t)year_days)
			break;
		days -= (uint32_t)year_days;
		year++;
	}

	yday = (int)days;
	month = 0;
	while (month < 12) {
		mdays = month_days[month];
		if (month == 1 && __nolibc_is_leap(year))
			mdays++;
		if (days < (uint32_t)mdays)
			break;
		days -= (uint32_t)mdays;
		month++;
	}

	out->tm_year = year - 1900;
	out->tm_yday = yday;
	out->tm_mon = month;
	out->tm_mday = (int)days + 1;
	out->tm_isdst = 0;

	return out;
}

static __inline__
double difftime(time_t time1, time_t time2)
{
	return (double)(time1 - time2);
}

static __inline__ __attribute__((unused, always_inline))
time_t time(time_t *tptr)
{
	uint64_t ms = __nolibc_rars_time_ms();
	time_t seconds;

#if defined(__riscv_xlen) && (__riscv_xlen == 64)
	seconds = (time_t)(ms / 1000);
#else
	seconds = (time_t)((uint32_t)ms / 1000u);
#endif

	if (tptr)
		*tptr = seconds;
	return seconds;
}

static __inline__ __attribute__((unused))
struct tm *gmtime(const time_t *timer)
{
	static struct tm out;

	if (!timer)
		return NULL;
	return __nolibc_gmtime_utc(*timer, &out);
}

static __inline__ __attribute__((unused))
struct tm *localtime(const time_t *timer)
{
	return gmtime(timer);
}

static __inline__ __attribute__((unused))
char *asctime(const struct tm *timeptr)
{
	static char buf[26];
	static const char *wday_name[7] = {
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};
	static const char *mon_name[12] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};
	int year;

	if (!timeptr)
		return NULL;

	__nolibc_copy3(buf + 0, wday_name[timeptr->tm_wday % 7]);
	buf[3] = ' ';
	__nolibc_copy3(buf + 4, mon_name[timeptr->tm_mon % 12]);
	buf[7] = ' ';
	__nolibc_write_2d(buf + 8, timeptr->tm_mday, 1);
	buf[10] = ' ';
	__nolibc_write_2d(buf + 11, timeptr->tm_hour, 0);
	buf[13] = ':';
	__nolibc_write_2d(buf + 14, timeptr->tm_min, 0);
	buf[16] = ':';
	__nolibc_write_2d(buf + 17, timeptr->tm_sec, 0);
	buf[19] = ' ';
	year = timeptr->tm_year + 1900;
	__nolibc_write_4d(buf + 20, year);
	buf[24] = '\n';
	buf[25] = '\0';

	return buf;
}

static __inline__ __attribute__((unused))
char *ctime(const time_t *timer)
{
	struct tm *tm;

	if (!timer)
		return NULL;
	tm = localtime(timer);
	if (!tm)
		return NULL;
	return asctime(tm);
}

static __inline__
int __nolibc_days_before_year(int year)
{
	int days = 0;
	int y;

	for (y = 1970; y < year; y++)
		days += __nolibc_is_leap(y) ? 366 : 365;
	return days;
}

static __inline__
int __nolibc_month_length(int year, int month)
{
	static const int month_days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int days = month_days[month];

	if (month == 1 && __nolibc_is_leap(year))
		days += 1;
	return days;
}

static __inline__
int __nolibc_days_before_month(int year, int month)
{
	int days = 0;
	int i;

	for (i = 0; i < month; i++) {
		days += __nolibc_month_length(year, i);
	}
	return days;
}

static __inline__ __attribute__((unused))
time_t mktime(struct tm *timeptr)
{
	int year;
	int month;
	int mday;
	int hour;
	int min;
	int sec;
	int days;
	uint32_t total_seconds;

	if (!timeptr)
		return (time_t)-1;

	year = timeptr->tm_year + 1900;
	month = timeptr->tm_mon;
	mday = timeptr->tm_mday;
	hour = timeptr->tm_hour;
	min = timeptr->tm_min;
	sec = timeptr->tm_sec;

	if (year < 1970 || month < 0 || month > 11)
		return (time_t)-1;
	if (mday < 1 || mday > __nolibc_month_length(year, month))
		return (time_t)-1;
	if (hour < 0 || hour > 23 || min < 0 || min > 59 || sec < 0 || sec > 60)
		return (time_t)-1;

	days = __nolibc_days_before_year(year) + __nolibc_days_before_month(year, month) + (mday - 1);
	total_seconds = (uint32_t)days * 86400u + (uint32_t)hour * 3600u + (uint32_t)min * 60u + (uint32_t)sec;

	__nolibc_gmtime_utc((time_t)total_seconds, timeptr);
	return (time_t)total_seconds;
}

static __inline__
int __nolibc_str_append(char *s, size_t max, size_t *pos, const char *src, size_t len)
{
	size_t i;

	if (*pos + len >= max)
		return 0;
	for (i = 0; i < len; i++)
		s[*pos + i] = src[i];
	*pos += len;
	return 1;
}

static __inline__ __attribute__((unused))
size_t strftime(char *s, size_t max, const char *format, const struct tm *timeptr)
{
	static const char *wday_name[7] = {
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};
	static const char *mon_name[12] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};
	size_t pos = 0;
	char tmp[24];
	int year;

	if (!s || !format || !timeptr || max == 0)
		return 0;

	while (*format) {
		if (*format != '%') {
			if (!__nolibc_str_append(s, max, &pos, format, 1))
				return 0;
			format++;
			continue;
		}

		format++;
		if (*format == '\0')
			break;

		switch (*format) {
		case '%':
			if (!__nolibc_str_append(s, max, &pos, "%", 1))
				return 0;
			break;
		case 'Y':
			year = timeptr->tm_year + 1900;
			__nolibc_write_4d(tmp, year);
			if (!__nolibc_str_append(s, max, &pos, tmp, 4))
				return 0;
			break;
		case 'm':
			__nolibc_write_2d(tmp, timeptr->tm_mon + 1, 0);
			if (!__nolibc_str_append(s, max, &pos, tmp, 2))
				return 0;
			break;
		case 'd':
			__nolibc_write_2d(tmp, timeptr->tm_mday, 0);
			if (!__nolibc_str_append(s, max, &pos, tmp, 2))
				return 0;
			break;
		case 'H':
			__nolibc_write_2d(tmp, timeptr->tm_hour, 0);
			if (!__nolibc_str_append(s, max, &pos, tmp, 2))
				return 0;
			break;
		case 'M':
			__nolibc_write_2d(tmp, timeptr->tm_min, 0);
			if (!__nolibc_str_append(s, max, &pos, tmp, 2))
				return 0;
			break;
		case 'S':
			__nolibc_write_2d(tmp, timeptr->tm_sec, 0);
			if (!__nolibc_str_append(s, max, &pos, tmp, 2))
				return 0;
			break;
		case 'a':
			if (!__nolibc_str_append(s, max, &pos, wday_name[timeptr->tm_wday % 7], 3))
				return 0;
			break;
		case 'b':
			if (!__nolibc_str_append(s, max, &pos, mon_name[timeptr->tm_mon % 12], 3))
				return 0;
			break;
		case 'c':
			__nolibc_copy3(tmp + 0, wday_name[timeptr->tm_wday % 7]);
			tmp[3] = ' ';
			__nolibc_copy3(tmp + 4, mon_name[timeptr->tm_mon % 12]);
			tmp[7] = ' ';
			__nolibc_write_2d(tmp + 8, timeptr->tm_mday, 1);
			tmp[10] = ' ';
			__nolibc_write_2d(tmp + 11, timeptr->tm_hour, 0);
			tmp[13] = ':';
			__nolibc_write_2d(tmp + 14, timeptr->tm_min, 0);
			tmp[16] = ':';
			__nolibc_write_2d(tmp + 17, timeptr->tm_sec, 0);
			tmp[19] = ' ';
			year = timeptr->tm_year + 1900;
			__nolibc_write_4d(tmp + 20, year);
			if (!__nolibc_str_append(s, max, &pos, tmp, 24))
				return 0;
			break;
		default:
			if (!__nolibc_str_append(s, max, &pos, format, 1))
				return 0;
			break;
		}
		format++;
	}

	s[pos] = '\0';
	return pos;
}

#endif /* _NOLIBC_TIME_H */
