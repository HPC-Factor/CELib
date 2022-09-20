// bsd.c
//
// Time-stamp: <31/01/01 21:13:00 keuchel@lightning>

#include <stdlib.h>
#include <string.h>
#include "celib.h"

void *
bcopy(void *from, void *to, size_t size)
{
  // handles overlapping
  return memmove(to, from, size);
}

int
bcmp(const void *a1, const void *a2, size_t size)
{
  return memcmp(a1, a2, size);
}

void *
bzero(void *block, size_t size)
{
  return memset(block, 0, size);
}

char *
index(const char *s, int c)
{
  return strchr(s, c);
}

char *
rindex(const char *s, int c)
{
  return xcestrrchr(s, c);
}


