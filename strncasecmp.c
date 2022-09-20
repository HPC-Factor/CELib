// strncasecmp.c
//
// Time-stamp: <31/01/01 21:21:31 keuchel@lightning>

#include "celib.h"

int
strncasecmp(const char *s1, const char *s2, int n)
{
  return xcestrnicmp(s1, s2, n);
}
