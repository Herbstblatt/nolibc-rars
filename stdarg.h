/* SPDX-License-Identifier: LGPL-2.1 OR MIT */
/*
 * Variadic argument support for NOLIBC
 * Copyright (C) 2005-2020 Rich Felker, et al.
 */

#ifndef _NOLIBC_STDARG_H
#define _NOLIBC_STDARG_H

#define va_start(ap, param) __builtin_va_start(ap, param)
#define va_end(ap)          __builtin_va_end(ap)
#define va_arg(ap, type)    __builtin_va_arg(ap, type)

typedef __builtin_va_list va_list;

#endif /* _NOLIBC_STDARG_H */
