// snprintf.c
//
// Time-stamp: <31/01/01 21:21:11 keuchel@lightning>

#include <stdarg.h>
#include <stdio.h>

int 
xcesnprintf(char *buf, size_t count, const char *fmt, ...)
{
  va_list ap;
  int n = 0;

  va_start(ap, fmt);
  n = _vsnprintf(buf, count, fmt, ap);
  va_end(ap);
  return n;
}
