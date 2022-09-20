// ceprocess.h
//
// Time-stamp: <28/01/01 18:24:04 keuchel@lightning>

#include "celib.h"
#include "sys/stat.h"

int
XCESearchPathA(const char *pathlist, const char *file, char *retpath)
{
  char path[MAX_PATH];
  char *ps, *pe;
  char *pl;
  struct xcestat st;
  char *pend;

  if(pathlist == NULL)
    return -1;

  ps = pe = pl = xcestrdup(pathlist);
  pend = pl + strlen(pl);

  while(ps < pend)
    {
      while(*pe && *pe != ';')
	pe++;
      *pe++ = 0;

      sprintf(path, "%s\\%s", ps, file);

      //printf("SearchPath: %s\n", path);

      if(xcestat(path, &st) == 0)
	{
	  strcpy(retpath, path);
	  free(pl);
	  return 0;
	}
      ps = pe;
    }

  free(pl);
  return -1;
}

int
XCEExecuteProcessA(const char *commandline, BOOL bWait, LPDWORD lpdwProcId)
{
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  wchar_t wszAppName[256];
  char szAppName[126];
  wchar_t wszCommandLine[256];
  char szCommandLine[256];
  char *p;
  DWORD dwExitCode = 0;
  char path[MAX_PATH];
  char *d;
  char szCurrentDir[MAX_PATH];

  memset(&si, 0, sizeof(si));
  si.cb = sizeof(si);

  for(p = commandline, d = szAppName; *p && *p != ' ';)
    *d++ = *p++;
  *d++ = 0;

  while(*p == ' ')
    p++;
  strcpy(szCommandLine, p);

  for(p = szAppName; *p; p++)
    {
      if(*p == '/')
	*p = '\\';
    }

  if(szAppName[0] != '\\')
    {
      if(strstr(szAppName, ".exe") == NULL)
	strcat(szAppName, ".exe");

      if(XCESearchPathA(xcegetenv("PATH"), szAppName, path) == 0)
	strcpy(szAppName, path);
    }

  //printf("App: %s Command: %s\n", szAppName, szCommandLine);

  MultiByteToWideChar(CP_ACP, 0, szAppName, -1,
                      wszAppName, sizeof(wszAppName)/2);
  MultiByteToWideChar(CP_ACP, 0, szCommandLine, -1,
                      wszCommandLine, sizeof(wszCommandLine)/2);

  // pass path to child...
  XCEGetCurrentDirectoryA(sizeof(szCurrentDir), szCurrentDir);
  XCESetEnvironmentVariableInRegA("CHILDPATH", szCurrentDir);
  // PWD is better, as this is checked by some unix programs...
  XCESetEnvironmentVariableInRegA("PWD", szCurrentDir);

  if(CreateProcessW(wszAppName, wszCommandLine, NULL, NULL,
                     FALSE, 0, NULL, NULL, &si, &pi) == FALSE)
    {
      return -1;
    }

  if(bWait)
    {
      WaitForSingleObject(pi.hProcess, INFINITE);
      GetExitCodeProcess(pi.hProcess, &dwExitCode);
    }
  else
    {
      if(lpdwProcId)
	*lpdwProcId = pi.dwProcessId;
    }

  CloseHandle(pi.hThread);
  CloseHandle(pi.hProcess);

  return dwExitCode;
}

int
XCEWaitProcess(DWORD dwProcId)
{
  HANDLE hProcess;
  DWORD dwExitCode;

  if((hProcess = OpenProcess(0, FALSE, dwProcId)) == NULL)
    return -1;

  WaitForSingleObject(hProcess, INFINITE);
  GetExitCodeProcess(hProcess, &dwExitCode);

  CloseHandle(hProcess);

  return dwExitCode;
}
