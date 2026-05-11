/*
 * Special types used by various syscalls for NOLIBC
 * Copyright (C) 2017-2021 Willy Tarreau <w@1wt.eu>
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

#ifndef _NOLIBC_TYPES_H
#define _NOLIBC_TYPES_H

#include "std.h"


/* Only the generic macros and types may be defined here. The arch-specific
 * ones such as the O_RDONLY and related macros used by fcntl() and open()
 * must not be defined here.
 */

/* stat flags (WARNING, octal here). We need to check for an existing
 * definition because linux/stat.h may omit to define those if it finds
 * that any glibc header was already included.
 */
#if !defined(S_IFMT)
#define S_IFDIR        0040000
#define S_IFCHR        0020000
#define S_IFBLK        0060000
#define S_IFREG        0100000
#define S_IFIFO        0010000
#define S_IFLNK        0120000
#define S_IFSOCK       0140000
#define S_IFMT         0170000

#define S_ISDIR(mode)  (((mode) & S_IFMT) == S_IFDIR)
#define S_ISCHR(mode)  (((mode) & S_IFMT) == S_IFCHR)
#define S_ISBLK(mode)  (((mode) & S_IFMT) == S_IFBLK)
#define S_ISREG(mode)  (((mode) & S_IFMT) == S_IFREG)
#define S_ISFIFO(mode) (((mode) & S_IFMT) == S_IFIFO)
#define S_ISLNK(mode)  (((mode) & S_IFMT) == S_IFLNK)
#define S_ISSOCK(mode) (((mode) & S_IFMT) == S_IFSOCK)

#define S_IRWXU 00700
#define S_IRUSR 00400
#define S_IWUSR 00200
#define S_IXUSR 00100

#define S_IRWXG 00070
#define S_IRGRP 00040
#define S_IWGRP 00020
#define S_IXGRP 00010

#define S_IRWXO 00007
#define S_IROTH 00004
#define S_IWOTH 00002
#define S_IXOTH 00001
#endif

/* dirent types */
#define DT_UNKNOWN     0x0
#define DT_FIFO        0x1
#define DT_CHR         0x2
#define DT_DIR         0x4
#define DT_BLK         0x6
#define DT_REG         0x8
#define DT_LNK         0xa
#define DT_SOCK        0xc

/* PATH_MAX and MAXPATHLEN are often used and found with plenty of different
 * values.
 */
#ifndef PATH_MAX
#define PATH_MAX       4096
#endif

#ifndef MAXPATHLEN
#define MAXPATHLEN     (PATH_MAX)
#endif

/* flags for mmap */
#ifndef MAP_FAILED
#define MAP_FAILED ((void *)-1)
#endif

/* whence values for lseek() */
#define SEEK_SET       0
#define SEEK_CUR       1
#define SEEK_END       2

/* flags for reboot */
#define RB_AUTOBOOT     LINUX_REBOOT_CMD_RESTART
#define RB_HALT_SYSTEM  LINUX_REBOOT_CMD_HALT
#define RB_ENABLE_CAD   LINUX_REBOOT_CMD_CAD_ON
#define RB_DISABLE_CAD  LINUX_REBOOT_CMD_CAD_OFF
#define RB_POWER_OFF    LINUX_REBOOT_CMD_POWER_OFF
#define RB_SW_SUSPEND   LINUX_REBOOT_CMD_SW_SUSPEND
#define RB_KEXEC        LINUX_REBOOT_CMD_KEXEC

/* standard exit() codes */
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

typedef int clockid_t;
typedef int timer_t;

#ifndef container_of
#define container_of(PTR, TYPE, FIELD) ({			\
	__typeof__(((TYPE *)0)->FIELD) *__FIELD_PTR = (PTR);	\
	(TYPE *)((char *) __FIELD_PTR - offsetof(TYPE, FIELD));	\
})
#endif

#endif /* _NOLIBC_TYPES_H */
