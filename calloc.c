// calloc.c
//
// Time-stamp: <31/01/01 21:13:24 keuchel@lightning>

#include "celib.h"

void *
calloc(size_t num, size_t size)
{
  void *p;

  size *= num;

  if((p = malloc(size)) == NULL)
    return NULL;

  memset(p, 0, size);

  return p;
}
