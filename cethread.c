// cethread.c
//
// Time-stamp: <03/02/01 10:11:45 keuchel@lightning>

#include "celib.h"
#include "cethread.h"

unsigned long __tlsindex = 0xffffffff;

unsigned long WINAPI _threadstart(void *);

void _freeptd(_ptiddata);
void _initptd(_ptiddata);

void
_init_multithread()
{
  if((__tlsindex = TlsAlloc()) == 0xFFFFFFFF)
    xceabort();
}

unsigned long 
_beginthread(void (* initialcode) (void *), unsigned stacksize, 
	     void * argument)
{
  _ptiddata ptd;
  unsigned long thdl;
  unsigned long errcode = 0L;

  if(__tlsindex == 0xFFFFFFFF)
    _init_multithread();

  if ( (ptd = (void *) calloc(1, sizeof(struct _tiddata))) == NULL )
    goto error_return;

  _initptd(ptd);

  ptd->_initaddr = (void *) initialcode;
  ptd->_initarg = argument;

  if ( (ptd->_thandle = thdl = (unsigned long)
	CreateThread( NULL,
		      stacksize,
		      _threadstart,
		      (LPVOID)ptd,
		      CREATE_SUSPENDED,
		      (LPDWORD)&(ptd->_tid) )) == 0L )
    {
      errcode = GetLastError();
      goto error_return;
    }

  if ( ResumeThread( (HANDLE)thdl ) == (DWORD)(-1L) ) {
    errcode = GetLastError();
    goto error_return;
  }

  return(thdl);

 error_return:
  free(ptd);

  return((unsigned long)-1L);
}

static unsigned long 
_threadstart (void * ptd)
{
  if ( !TlsSetValue(__tlsindex, ptd) )
    xceabort();

  ((void(__cdecl *)(void *))(((_ptiddata)ptd)->_initaddr))
    (((_ptiddata)ptd)->_initarg );
	
  _endthread();

  // not reached
  return(0L);
}

void 
_endthread (void)
{
  _ptiddata ptd;

  if((ptd = _getptd()) == NULL )
    xceabort();

  if(ptd->_thandle != (unsigned long)(-1L))
    CloseHandle((HANDLE)(ptd->_thandle));

  _freeptd(ptd);

  ExitThread(0);
}

void 
_initptd (_ptiddata ptd)
{
}

void 
_freeptd (_ptiddata ptd)
{
  if ( __tlsindex != 0xFFFFFFFF ) 
    {
      if (!ptd)
	{
	  ptd = TlsGetValue(__tlsindex);
	}

      if(ptd) 
	{
	  free((void *)ptd);
	}

      TlsSetValue(__tlsindex, (LPVOID) 0);
    }
}

_ptiddata 
_getptd (void)
{
  _ptiddata ptd;
  DWORD TL_LastError;

  TL_LastError = GetLastError();

  if ((ptd = TlsGetValue(__tlsindex)) == NULL) 
    {
      if(((ptd = (void*) calloc(1, sizeof(struct _tiddata))) != NULL) &&
	 TlsSetValue(__tlsindex, (LPVOID)ptd)) 
	{
	  _initptd(ptd);

	  ptd->_tid = GetCurrentThreadId();
	  ptd->_thandle = (unsigned long)(-1L);
	}
      else
	{
	  xceabort();
	}
    }

  SetLastError(TL_LastError);

  return(ptd);
}
