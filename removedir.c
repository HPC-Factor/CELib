// removedir.c
//
// Time-stamp: <04/01/01 12:15:39 keuchel@lightning>

#include "celib.h"

BOOL
XCERemoveDirectoryA(const char *lpName)
{
  wchar_t *lpNameNew = NULL;
  int len = strlen(lpName) + 1;
  BOOL res;

  lpNameNew = malloc(len * 2);
  MultiByteToWideChar(CP_ACP, 0, lpName, -1, lpNameNew, len);

  res = XCERemoveDirectoryW(lpNameNew);

  free(lpNameNew);

  return res;
}

BOOL
XCERemoveDirectoryW(const wchar_t *oldpath)
{
  wchar_t newpath[MAX_PATH];

  XCEFixPathW(oldpath, newpath);
  return RemoveDirectoryW(newpath);
}
