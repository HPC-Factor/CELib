// cedebug.c
//
// Time-stamp: <02/02/01 18:22:50 keuchel@lightning>

#include "celib.h"

void
XCETrace(const char *fmt, ...)
{
  FILE *f;

  if((f = fopen("/temp/xce.log", "a")))
    {
      va_list ap;

      va_start(ap, fmt);
      vfprintf(f, fmt, ap);
      va_end(ap);

      fclose(f);
    }
}

void
XCETraceW(const wchar_t *fmt, ...)
{
  FILE *f;

  if((f = fopen("/temp/xce.log", "a")))
    {
      va_list ap;

      va_start(ap, fmt);
      vfwprintf(f, fmt, ap);
      va_end(ap);

      fclose(f);
    }
}
