// createfile.c
//
// Time-stamp: <04/01/01 12:14:39 keuchel@lightning>

#include "celib.h"

HANDLE
XCECreateFileA(
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

  hFile = XCECreateFileW(wfname, dwDesiredAccess, dwShareMode,
			 lpSecurityAttributes, dwCreationDisposition,
			 dwFlagsAndAttributes, hTemplateFile);

  return hFile;
}

HANDLE
XCECreateFileW(
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

  hFile = CreateFileW(wpath, dwDesiredAccess, dwShareMode,
		      lpSecurityAttributes, dwCreationDisposition,
		      dwFlagsAndAttributes, hTemplateFile);

  if(hFile == INVALID_HANDLE_VALUE)
    {
      dwError = GetLastError();

      if(dwError == ERROR_FILE_NOT_FOUND ||
	 dwError == ERROR_PATH_NOT_FOUND)
	{
	}
      else
	{
	  fwprintf(stderr, L"WARNING: File problem: file=%s error=%d\n", 
		   wpath, dwError);
	}

      errno = _winerror2errno(dwError);
    }

  return hFile;
}
