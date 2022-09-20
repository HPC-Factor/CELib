/* Name: wsstart.c --- startup winsock
 *
 * Time-stamp: <28/01/01 19:28:43 keuchel@lightning>
 */

#include <windows.h>
#include <winsock.h>
#include "celib.h"

static WORD wVersionRequested;
static WSADATA wsaData;

int
xcewsstart()
{
#ifndef WIN311	
  wVersionRequested = MAKEWORD(2, 0);
#else
  wVersionRequested = 0x0101;
#endif  
  // returns zero on success
  return(WSAStartup(wVersionRequested, &wsaData));
}

void
xcewsstop()
{
  WSACleanup();
}
