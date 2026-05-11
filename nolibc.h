/*
 * Copyright (C) 2017-2018 Willy Tarreau <w@1wt.eu>
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

/*
 * This file is designed to be used as a libc alternative for minimal programs
 * with very limited requirements. It consists of a small number of syscall and
 * type definitions, and the minimal startup code needed to call main().
 * All syscalls are declared as static functions so that they can be optimized
 * away by the compiler when not used.
 *
 * The errno variable is declared static and unused. This way it can be
 * optimized away if not used. However this means that a program made of
 * multiple C files may observe different errno values (one per C file). For
 * the type of programs this project targets it usually is not a problem. The
 * resulting program may even be reduced by defining the NOLIBC_IGNORE_ERRNO
 * macro, in which case the errno value will never be assigned.
 *
 * Some stdint-like integer types are defined. These are valid on all currently
 * supported architectures, because signs are enforced, ints are assumed to be
 * 32 bits, longs the size of a pointer and long long 64 bits. If more
 * architectures have to be supported, this may need to be adapted.
 *
 * Finally some very common libc-level functions are provided. It is the case
 * for a few functions usually found in string.h, ctype.h, or stdlib.h.
 *
 * The nolibc.h file is only a convenient entry point which includes all other
 * files. It also defines the NOLIBC macro, so that it is possible for a
 * program to check this macro to know if it is being built against and decide
 * to disable some features or simply not to include some standard libc files.
 *
 * The available standard (but limited) include files are:
 *   ctype.h, errno.h, stdarg.h, stdio.h, stdlib.h,
 *   string.h, time.h
 *
 * In addition, the following ones are expected to be provided by the compiler:
 *   float.h, stddef.h
 *
 * The following ones which are part to the C standard are not provided:
 *   assert.h, locale.h, math.h, setjmp.h, limits.h
 *
 */
#ifndef _NOLIBC_H
#define _NOLIBC_H

#include "std.h"
#include "rars.h"
#include "types.h"
#include "ctype.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "getopt.h"
#include "math.h"

/* Used by programs to avoid std includes */
#define NOLIBC

#endif /* _NOLIBC_H */
