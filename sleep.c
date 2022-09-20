#include "celib.h"

unsigned int
sleep(unsigned int secs)
{
  Sleep(secs * 1000);
  return 0;
}

