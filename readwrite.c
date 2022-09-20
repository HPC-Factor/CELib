// readwrite.c
//
// Time-stamp: <04/01/01 11:40:05 keuchel@lightning>

#include "celib.h"

BOOL 
XCEWriteFile(
	     HANDLE hFile,
	     LPCVOID lpBuffer,
	     DWORD nNumberOfBytesToWrite,
	     LPDWORD lpNumberOfBytesWritten,
	     LPOVERLAPPED lpOverlapped
	     )
{
  return WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite,
		   lpNumberOfBytesWritten, lpOverlapped);
}

BOOL 
XCEReadFile(
	    HANDLE hFile,
	    LPVOID lpBuffer,
	    DWORD nNumberOfBytesToRead,
	    LPDWORD lpNumberOfBytesRead,
	    LPOVERLAPPED lpOverlapped
	    )
{
  return ReadFile(hFile, lpBuffer, nNumberOfBytesToRead,
		  lpNumberOfBytesRead, lpOverlapped);
}
