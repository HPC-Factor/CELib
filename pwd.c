// pwd.c - password routines etc
//
// Time-stamp: <31/01/01 22:08:49 keuchel@lightning>

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <errno.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifndef UNDER_CE
#pragma comment (lib, "advapi32.lib")
#endif

#include "pwd.h"

#ifdef UNDER_CE
#define __declspec(X)
#endif

__declspec(thread) static FILE *fpw;
__declspec(thread) static struct passwd pw;
__declspec(thread) static char pwbuf[1024];
__declspec(thread) static char shell[256];
__declspec(thread) static char dir[256];
__declspec(thread) static char passbuf[126];
__declspec(thread) static char loginbuf[126];

void 
setpwent(void)
{
  char path[MAX_PATH];

  XCEGetEnvironmentVariableFromRegA("UNIXROOTDIR", path, sizeof(path));
  strcat(path, "\\etc\\passwd");

  fpw = fopen(path, "rt");
}

struct passwd *
getpwent(void)
{
  char *delim = ":";
  char *p;
  char *s;
  
  if(fpw == NULL)
    return NULL;

  if(feof(fpw))
    return NULL;

  if(fgets(pwbuf, sizeof(pwbuf), fpw) == NULL)
    return NULL;

  if((p = strchr(pwbuf, '\n')))
    *p = 0;
  else
    return NULL;

  if(pwbuf[0] == 0)
    return NULL;

  p = pwbuf;

  pw.pw_name = p; 
  p = strchr(p, ':'); if(!p) return NULL; *p++ = 0;

  pw.pw_passwd = p;
  p = strchr(p, ':'); if(!p) return NULL; *p++ = 0;

  s = p;
  p = strchr(p, ':'); if(!p) return NULL; *p++ = 0;
  pw.pw_uid = atoi(s);
  
  s = p;
  p = strchr(p, ':'); if(!p) return NULL; *p++ = 0;
  pw.pw_gid = atoi(s);

  pw.pw_comment = p;
  p = strchr(p, ':'); if(!p) return NULL; *p++ = 0;

  pw.pw_dir = p;
  p = strchr(p, ':'); if(!p) return NULL; *p++ = 0;

  pw.pw_shell = p;

#ifdef DEBUGX
  printf("Name:        %s\n", pw.pw_name);
  printf("Pass:        %s\n", pw.pw_passwd);
  printf("Uid:         %d\n", pw.pw_uid);
  printf("Gid:         %d\n", pw.pw_gid);
  printf("Comment:     %s\n", pw.pw_comment);
  printf("Directory:   %s\n", pw.pw_dir);
  printf("Shell:       %s\n", pw.pw_shell);
#endif

  pw.pw_gecos = "";
  return &pw;
}

void
endpwent(void)
{
  if(fpw)
    fclose(fpw);
}

/* get passwd entry by username */
struct passwd *
getpwnam(const char *name)
{
  struct passwd *pw = NULL;

  setpwent();
  while((pw = getpwent()) != NULL)
    {
      if(strcmp(name, pw->pw_name) == 0)
	break;
    }
  endpwent();
  return pw;
}

/* get passwd entry by username */
struct passwd *
getpwuid(uid_t uid)
{
  struct passwd *pw = NULL;

  setpwent();
  while((pw = getpwent()) != NULL)
    {
      if(uid == pw->pw_uid)
	break;
    }
  endpwent();
  return pw;
}

char *
getusershell()
{
  // must return NULL (read /etc/shells)
  return "sh.exe";
}

void
endusershell()
{
}

int
putpwent(struct passwd *pe, FILE *f)
{
  char *p;
  char *dir = pe->pw_dir;
  char *shell = pe->pw_shell;

  if(pe->pw_comment[0] != '*')
    {
      if(dir[0] != '/' && dir[1] == ':')
	dir += 2;
      
      if(shell[0] != '/' && shell[1] == ':')
	shell += 2;
    }

  return fprintf(f, "%s:%s:%d:%d:%s:%s:%s\n",
		 pe->pw_name,
		 pe->pw_passwd,
		 pe->pw_uid,
		 pe->pw_gid,
		 pe->pw_comment,
		 dir,
		 shell);
}

char *
getpass(char *prompt)
{
  return NULL;
}

char *
getlogin()
{
  char *p;
  DWORD len = sizeof(loginbuf);

  if((p = xcegetenv("USERNAME")) != NULL)
    strcpy(loginbuf, p);
  else
    XCEGetUserNameA(loginbuf, &len);

  return loginbuf;
}
