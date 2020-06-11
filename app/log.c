#include <stdio.h>
#include <stdarg.h>

void write_log(char *file, char *func, int line, int level, const char *format, ...)
{
    va_list ap;

    fprintf(stdout, "%s,%s:%d,", file, func, line);

    va_start(ap, format);
    vfprintf(stdout, format, ap);
    va_end(ap);

    fprintf(stdout, "\n");
    return;
}