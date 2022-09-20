// fileattr.c
//
// Time-stamp: <04/01/01 11:35:20 keuchel@lightning>

#include "celib.h"

DWORD
XCEGetFileAttributesA(const char *fname)
{
  wchar_t wfname[MAX_PATH];
  DWORD dwRes;

  MultiByteToWideChar(CP_ACP, 0, fname, -1, wfname, COUNTOF(wfname));

  dwRes = XCEGetFileAttributesW(wfname);

  return dwRes;
}

BOOL
XCESetFileAttributesA(const char *fname, DWORD dwAttr)
{
  wchar_t wfname[MAX_PATH];
  BOOL res;

  MultiByteToWideChar(CP_ACP, 0, fname, -1, wfname, COUNTOF(wfname));

  res = XCESetFileAttributesW(wfname, dwAttr);

  return res;
}

DWORD
XCEGetFileAttributesW(const wchar_t *wfname)
{
  wchar_t wpath[MAX_PATH];
  DWORD dwRes;

  XCEFixPathW(wfname, wpath);
  dwRes = GetFileAttributesW(wpath);

  return dwRes;
}

BOOL
XCESetFileAttributesW(const wchar_t *wfname, DWORD dwAttr)
{
  wchar_t wpath[MAX_PATH];
  BOOL res;

  XCEFixPathW(wfname, wpath);
  res = SetFileAttributesW(wpath, dwAttr);

  return res;
}
