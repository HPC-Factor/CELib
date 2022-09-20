// createfilemap.c
//
// Time-stamp: <04/01/01 12:15:59 keuchel@lightning>

#include "celib.h"

HANDLE
XCECreateFileForMappingA(
	       LPCSTR fname,
	       DWORD dwDesiredAccess,
	       DWORD dwShareMode,
	       LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	       DWORD dwCreationDisposition,
	       DWORD dwFlagsAndAttributes,
	       HANDLE hTemplateFile
	       )
{
  wchar_t wfname[MAX_PATH];
  HANDLE hFile;

  MultiByteToWideChar(CP_ACP, 0, fname, -1, wfname, COUNTOF(wfname));

  hFile = XCECreateFileForMappingW(wfname, dwDesiredAccess, dwShareMode,
				   lpSecurityAttributes, dwCreationDisposition,
				   dwFlagsAndAttributes, hTemplateFile);

  return hFile;
}

HANDLE
XCECreateFileForMappingW(
    LPCWSTR wfname,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
    )
{
  HANDLE hFile;
  wchar_t wpath[MAX_PATH];
  wchar_t *p;
  DWORD dwError = 0;

  XCEFixPathW(wfname, wpath);

  hFile = CreateFileForMappingW(wpath, dwDesiredAccess, dwShareMode,
		      lpSecurityAttributes, dwCreationDisposition,
		      dwFlagsAndAttributes, hTemplateFile);

  if(hFile == INVALID_HANDLE_VALUE)
    {
      errno = _winerror2errno(dwError);
    }

  return hFile;
}
