// createdir.c
//
// Time-stamp: <04/01/01 12:01:46 keuchel@lightning>

#include "celib.h"

BOOL
XCECreateDirectoryA(const char *lpName, 
		    LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
  wchar_t lpNameNew[MAX_PATH];
  BOOL res;

  MultiByteToWideChar(CP_ACP, 0, lpName, -1, lpNameNew, COUNTOF(lpNameNew));

  res = XCECreateDirectoryW(lpNameNew, lpSecurityAttributes);

  return res;
}

BOOL
XCECreateDirectoryW(const wchar_t *oldpath, LPSECURITY_ATTRIBUTES lpSec)
{
  wchar_t newpath[MAX_PATH];

  XCEFixPathW(oldpath, newpath);
  return CreateDirectoryW(newpath, lpSec);
}
