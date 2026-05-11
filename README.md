# nolibc-rars

This is a minimal C standard library subset for the RARS RISC-V simulator. It is
designed to make simple programs build and run without a full libc or OS, while
staying small and easy to reason about.

## Why it exists

RARS provides a small set of simulator syscalls and no operating system. A full
libc expects a kernel and rich syscalls, so this library implements just enough
to support simple programs on top of RARS.

## What is supported

- Core types and macros: std.h, stdint.h, stddef.h, types.h
- C string/memory utilities: string.h (mem*, str*, tolower/toupper)
- Basic I/O: stdio.h with RARS read/write/open/close syscalls
- Memory allocation: stdlib.h with sbrk-backed heap
- Time (UTC-only): time.h with time(), gmtime/localtime, asctime/ctime,
  strftime (minimal set), mktime
- Math helpers where used by the toolchain

## What is not supported

- POSIX-only time APIs (clock_gettime, nanosleep, timer_*)
- Locale, threading, signals, and most OS-dependent facilities
- Filesystem features beyond the RARS syscall set

## Notes

- Time uses RARS_Time and reports UTC only.
- Implementations favor simplicity and determinism over completeness.

## Licensing

This project is based on [nolibc project](https://github.com/wtarreau/nolibc) by Willy Tarreau, which is currently integrated into the Linux kernel. The original project is distributed under the MIT License, hence this project is distributed under it too. License notices are put on top of source files where neccessary.