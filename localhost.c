// localhost.c
//
// Time-stamp: <31/01/01 21:19:54 keuchel@lightning>

#define WIN32_LEAN_AND_MEAN
#include <winsock.h>
#include <malloc.h>

char *
localhost()
{
  char *name;

  if((name = malloc(256)) == NULL)
    return name;

  gethostname(name, 256);
  return name;
}
