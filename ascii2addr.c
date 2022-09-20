// ascii2addr.c
//
// Time-stamp: <03/01/01 22:45:26 keuchel@lightning>

#include <windows.h>
#include <winsock.h>
#include "celib.h"

int
ascii2addr(int af, const char *ascii, void *result)
{
  struct in_addr *ina;
  char strbuf[4*sizeof("123")]; /* long enough for V4 only */

  switch(af) 
    {
    case AF_INET:
      ina = (struct in_addr *) result;
      strbuf[0] = '\0';
      strncat(strbuf, ascii, (sizeof strbuf)-1);
      if (inet_aton(strbuf, ina))
        return sizeof(struct in_addr);
      WSASetLastError(WSAEINVAL);
      break;
    default:
      WSASetLastError(WSAEPROTONOSUPPORT);
      break;
    }

  return -1;
}
