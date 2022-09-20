// xcesignal.c
//
// Time-stamp: <28/01/01 20:27:30 keuchel@lightning>

#include "celib.h"
#include "signal.h"

void
_init_signals()
{
}

typedef void (* _PHNDLR)(int);

_PHNDLR 
xcesignal(int signum, _PHNDLR sigact)
{
  return SIG_DFL;
}
