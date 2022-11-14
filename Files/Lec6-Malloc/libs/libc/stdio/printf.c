#include <klibc/stdio.h>
#include <stdarg.h>
#include <lunaix/tty/tty.h>
// This is VERY bad implementation as it mixes both kernel and user space
// code together. It is here however, just for the convenience of our testing
// program.
// FIXME Eliminate this when we're able to load program.

void
printf(const char* fmt, ...)
{
    const char buf[512];
    va_list args;
    va_start(args, fmt);

    __ksprintf_internal(buf, fmt, 512, args);

    va_end(args);
    tty_put_str(buf);
}