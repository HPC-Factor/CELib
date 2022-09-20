// wserr.c
//
// Time-stamp: <03/01/01 21:33:04 keuchel@lightning>

#include "celib.h"

void
xcewsperror(char *s)
{
  fprintf(stderr, "%s: %s\n", s, wsstrerror(WSAGetLastError()));
}

void
xcewserr(int eval, const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);

  fprintf(stderr, "%s\n", xcewsstrerror(eval));

  va_end(ap);
}
