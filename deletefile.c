// deletefile.c
//
// Time-stamp: <04/01/01 12:14:54 keuchel@lightning>

#include "celib.h"

BOOL
XCEDeleteFileA(const char *fname)
{
  wchar_t wfname[MAX_PATH];

  MultiByteToWideChar(CP_ACP, 0, fname, -1, wfname, COUNTOF(wfname));

  return XCEDeleteFileW(wfname);
}

BOOL
XCEDeleteFileW(const wchar_t *wfname)
{
  wchar_t wpath[MAX_PATH];

  XCEFixPathW(wfname, wpath);

  return DeleteFileW(wpath);
}
