// ceutil.c
//
// Time-stamp: <31/01/01 21:18:38 keuchel@lightning>

#include "celib.h"

void
XCEShowMessageA(const char *fmt, ...)
{
  va_list ap;
  char buf[512];
  wchar_t wbuf[512];

  va_start(ap, fmt);
  vsprintf(buf, fmt, ap);
  va_end(ap);

  MultiByteToWideChar(CP_ACP, 0, buf, -1, wbuf, COUNTOF(wbuf));

  MessageBoxW(NULL, wbuf, _T("Message"), MB_OK);
}

void
XCEShowMessageW(const wchar_t *fmt, ...)
{
  va_list ap;
  wchar_t buf[512];

  va_start(ap, fmt);
  vswprintf(buf, fmt, ap);
  va_end(ap);

  MessageBoxW(NULL, buf, _T("Message"), MB_OK);
}
