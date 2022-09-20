// cert0.c - runtime init
//
// Time-stamp: <31/01/01 21:16:49 keuchel@lightning>

#include "celib.h"

int
wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, wchar_t *lpCommand, int nShow)
{
  xceinit(lpCommand);

  return main(__argc, __argv, environ);
}
