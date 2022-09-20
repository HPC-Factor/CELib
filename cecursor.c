// cecursor.c
//
// Time-stamp: <31/01/01 21:14:54 keuchel@lightning>

#include "celib.h"

void
XCEShowWaitCursor()
{
  HCURSOR hCursor;

  hCursor = LoadCursorW(NULL, MAKEINTRESOURCEW(IDC_WAIT));

  SetCursor(hCursor);
}

void
XCEShowNormalCursor()
{
  HCURSOR hCursor;

  hCursor = LoadCursorW(NULL, MAKEINTRESOURCEW(IDC_ARROW));

  SetCursor(hCursor);
}

