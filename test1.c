// test1.c

#include "celib.h"
#include "sys/stat.h"
#include "time.h"
#include "sys/timeb.h"
#include "cewinnt.h"
#include "dirent.h"

#ifdef _SH3_
// on sh3, inline assebly needs these protos...
void __asm(const char *, ...);
#endif

int
test_null()
{
  int fd;

  if((fd = xceopen("nul", O_RDONLY, 0)) < 0)
    printf("Cannot open nul - %s\n", xcestrerror(errno));

  if((fd = xceopen("nul:", O_RDONLY, 0)) < 0)
    printf("Cannot open nul: - %s\n", xcestrerror(errno));

  if((fd = xceopen("null:", O_RDONLY, 0)) < 0)
    printf("Cannot open null: - %s\n", xcestrerror(errno));

  return 0;
}

int
test_intresource()
{
  wchar_t *wp = MAKEINTRESOURCEW(127);
  
  printf("0x%x %d\n", wp, wp);
}

int
test_size()
{
  printf("int:       %d\n", sizeof(int));
  printf("long:      %d\n", sizeof(long));
  //printf("long long: %d\n", sizeof(long long));
  printf("double:    %d\n", sizeof(double));
  printf("long double: %d\n", sizeof(long double));
}

int
dump_virtual_memory(DWORD dwAddr)
{
  MEMORY_BASIC_INFORMATION mi;

  if(VirtualQuery((LPVOID) dwAddr, &mi, sizeof(mi)) == sizeof(mi))
    {
      int fd;

      fd = xceopen("\\speicherkarte2\\tmp\\memory.dat", 
		   O_BINARY|O_WRONLY|O_CREAT|O_TRUNC, 0666);
      xcewrite(fd, mi.BaseAddress, mi.RegionSize);
      xceclose(fd);
    }
  else
    {
      fprintf(stderr, "VirtualQuery: %d\n", GetLastError());
    }

  return 0;
}

int
show_virtual_memory()
{
  SYSTEM_INFO si;
  MEMORY_BASIC_INFORMATION mi;
  DWORD dwAddr;
  FILE *f;

  f = fopen("\\speicherkarte2\\tmp\\memory.txt", "w");

  GetSystemInfo(&si);
  fprintf(f, "Pagesize is 0x%x bytes\n", si.dwPageSize);

  for(dwAddr = 0; dwAddr < 0x7FFFFFFF;)
    {
      if(VirtualQuery((LPVOID) dwAddr, &mi, sizeof(mi)) == sizeof(mi))
	{
	  if(mi.State == MEM_FREE)
	    {
	      dwAddr += mi.RegionSize;
	      continue;
	    }
	  if(mi.State == MEM_RESERVE)
	    {
	      dwAddr += mi.RegionSize;
	      continue;
	    }

	  if(mi.Protect == PAGE_NOACCESS)
	    {
	      dwAddr += mi.RegionSize;
	      continue;
	    }

	  // lots of small regions...
	  if(mi.RegionSize <= si.dwPageSize * 2)
	    {
	      dwAddr += mi.RegionSize;
	      continue;
	    }

	  fprintf(f, "Addr: 0x%08x ", dwAddr);
	  fprintf(f, "Base: 0x%08x ", mi.BaseAddress);
	  fprintf(f, "Size: 0x%08x ", mi.RegionSize);
	  fprintf(f, "Protect: 0x%02x ", mi.Protect);
	  fprintf(f, "Type: 0x%02x ", mi.Type);
	  fprintf(f, "\n");

	  dwAddr += mi.RegionSize;
	}
      else
	{
	  dwAddr += si.dwPageSize;
	}
    }

  fclose(f);

  return 0;
}

int
show_virtual_memory2()
{
  SYSTEM_INFO si;
  MEMORY_BASIC_INFORMATION mi;
  DWORD dwAddr;
  FILE *f;

  f = fopen("\\speicherkarte2\\tmp\\memory.txt", "w");

  GetSystemInfo(&si);
  fprintf(f, "Pagesize is 0x%x bytes\n", si.dwPageSize);

  for(dwAddr = 0x16000000; dwAddr < 0x20000000;)
    {
      if(VirtualQuery((LPVOID) dwAddr, &mi, sizeof(mi)) == sizeof(mi))
	{
	  fprintf(f, "Addr: 0x%08x ", dwAddr);
	  fprintf(f, "Base: 0x%08x ", mi.BaseAddress);
	  fprintf(f, "AllocBase: 0x%08x ", mi.AllocationBase);
	  fprintf(f, "Size: 0x%08x ", mi.RegionSize);
	  fprintf(f, "State: 0x%02x ", mi.State);
	  fprintf(f, "Protect: 0x%02x ", mi.Protect);
	  fprintf(f, "Type: 0x%02x ", mi.Type);

	  if(mi.Type & PAGE_GUARD)
	    fprintf(f, "PAGE_GUARD ");

	  fprintf(f, "\n");

	  dwAddr = (DWORD) mi.BaseAddress + mi.RegionSize;
	}
      else
	{
	  dwAddr += si.dwPageSize;
	}
    }

  fclose(f);

  return 0;
}

int
show_virtual_memory_addr(DWORD dwAddr)
{
  SYSTEM_INFO si;
  MEMORY_BASIC_INFORMATION mi;

  GetSystemInfo(&si);
  //printf("Pagesize is 0x%x bytes\n", si.dwPageSize);

  if(VirtualQuery((LPVOID) dwAddr, &mi, sizeof(mi)) == sizeof(mi))
    {            
      printf("Addr: 0x%08x ", dwAddr);
      printf("Base: 0x%08x ", mi.BaseAddress);
      printf("AllocBase: 0x%08x ", mi.AllocationBase);
      printf("Size: 0x%08x ", mi.RegionSize);
      printf("Protect: 0x%02x ", mi.Protect);
      printf("Type: 0x%02x ", mi.Type);
      printf("\n");
    }
  else
    {
      fprintf(stderr, "VirtualQuery: %d\n", GetLastError());
    }

  return 0;
}

int
dump_coredll_exports()
{
  HMODULE hMod;
  FILE *fexp;
  FILE *fout;
  char buf[256];
  char *p;
  FARPROC pfn;

  fexp = xcefopen("/Speicherkarte2/tmp/coredll-exports.txt", "r");
  fout = xcefopen("/Speicherkarte2/tmp/coredll-symbols.txt", "w");
  hMod = XCELoadLibraryA("coredll.dll");

  while((fgets(buf, sizeof(buf), fexp)))
    {
      if(buf[0] == '#')
	continue;
      if((p = xcestrrchr(buf, '\n')))
	*p = 0;
      if((pfn = XCEGetProcAddressA(hMod, buf)))
	{
	  fprintf(fout, "0x%x: %s\n", pfn, buf);
	}
    }

  fclose(fout);
  fclose(fexp);

  return 0;
}

/* When subsystem is windowsce,2.0, the stack setting is generally
   ignored by wince 2.11 and set to 0x10000.
   When subsystem is windowsce,2.11, the stack flag has effects! */

#define MAXSTACK 0x50000

int
test_stack1()
{
  char test[MAXSTACK];

  int i;

  for(i = 0; i < MAXSTACK; i++)
    test[i] = 'X';

  return 0;
}

DWORD
get_stack_sh4()
{
  DWORD xsp[1];

#ifdef _SH3_
  __asm("mov.l SP, @r4", xsp);
  printf("0x%x\n", xsp);
#endif

  return xsp[0];
}

int
test_stack2()
{
  int i;
  int size = 1024;
  char *p;

  //get_stack_sh4();

  //show_virtual_memory2();

  for(i = 0; i < MAXSTACK; i += size)
    {
      printf("%x\n", i);
      p = _alloca(size);
      show_virtual_memory_addr((DWORD) p);
    }

  //show_virtual_memory2();

  return 0;
}

int
test_dir()
{
  DIR *dir;
  struct dirent *dent;

  xcechdir("/Speicherkarte2");

  if((dir = opendir("/Speicherkarte2/.")) == NULL)
    {
      fprintf(stderr, "opendir: errno = %d\n", errno);
      return -1;
    }

  while((dent = readdir(dir)))
    {
      printf("%s\n", dent->d_name);
    }

  closedir(dir);

  return 0;
}

int
test_curdir()
{
  char dir[MAX_PATH];
  struct xcestat st;

  XCEGetCurrentDirectoryA(sizeof(dir), dir);
  printf("CurrentDir is %s\n", dir);

  if(xcestat("emacs", &st) < 0)
    printf("emacs not found\n");

  XCESetCurrentDirectoryA("\\Speicherkarte2");
  XCEGetCurrentDirectoryA(sizeof(dir), dir);
  printf("CurrentDir is %s\n", dir);

  if(xcestat("emacs", &st) < 0)
    printf("emacs not found\n");
}

int
test_process()
{
  int res;
  DWORD dwProcId;

  //res = XCEExecuteProcessA("rsh.exe lightning ls", TRUE, NULL);
  //printf("rsh returned %d\n", res);

  res = XCEExecuteProcessA("rsh.exe lightning ls", FALSE, &dwProcId);
  printf("rsh returned %d\n", res);
  res = XCEWaitProcess(dwProcId);
  XCEShowMessageA("wait returned %d\n", res);
}

int
test_image()
{
  HANDLE hFile;
  HANDLE hMap;
  void *file_base;
  DWORD dwCreateFlags;
  char *filename = "\\test1.exe";
  PIMAGE_DOS_HEADER pdh;
  int size = 1024;
  LPVOID lpAddr;

  dwCreateFlags = OPEN_EXISTING;

  if((hFile = XCECreateFileForMappingA(filename, 
				       GENERIC_READ,
				       FILE_SHARE_READ|FILE_SHARE_WRITE, 
				       NULL,
				       dwCreateFlags, 
				       FILE_ATTRIBUTE_NORMAL, NULL))
     == INVALID_HANDLE_VALUE)
    {
      printf("CreateFileForMapping: %d\n", GetLastError());
      return -1;
    }

  if((hMap = XCECreateFileMappingA(hFile, NULL, PAGE_READONLY, 0, size, NULL))
     == NULL)
    {
      printf("CreateFileMapping: %d\n", GetLastError());
      return -1;
    }

  if((file_base = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, size))
     == NULL)
    {
      printf("MapViewOfFile: %d\n", GetLastError());
      return -1;
    }

  pdh = (PIMAGE_DOS_HEADER) file_base;
  printf("Magic:   0x%x\n", pdh->e_magic); 

  lpAddr = (void *) 0x10000;

  if((lpAddr = VirtualAlloc(lpAddr, 0x400, MEM_COMMIT, 
			    PAGE_READWRITE)) == NULL)
    {
      printf("VirtualAlloc: %d\n", GetLastError());
    }

  memcpy(lpAddr, file_base, 0x400);

  pdh = (PIMAGE_DOS_HEADER) 0x10000;
  printf("Magic:   0x%x\n", pdh->e_magic); 

  UnmapViewOfFile(file_base);

  CloseHandle(hMap);
  CloseHandle(hFile);

  return 0;
}

int
test_toolhelp()
{
  DWORD base;
  PIMAGE_DOS_HEADER pdh;
  HMODULE hMod;
  MEMORY_BASIC_INFORMATION mi;
  DWORD dwOldProtect = 0;

  //XCEEnumProcesses();
  //XCEEnumModules();

  hMod = XCEGetModuleHandleA(NULL);
  DebugBreak();

  printf("hMod = 0x%x\n", hMod);

  // this is an address not in our address space!?
  base = XCEGetProcessBaseAddress(GetCurrentProcessId());
  pdh = (PIMAGE_DOS_HEADER) base;

  pdh = (PIMAGE_DOS_HEADER) 0x10000;

  VirtualQuery(pdh, &mi, sizeof(mi));
  
  printf("Base:    0x%x\n", mi.AllocationBase);
  printf("Size:    0x%x\n", mi.RegionSize);

  // this has normally PAGE_NOACCESS (0x1)
  printf("AllocProtect: 0x%x\n", mi.AllocationProtect);
  printf("Protect:      0x%x\n", mi.Protect);
  // MEM_RESERVE!
  printf("State:        0x%x\n", mi.State);

  if(VirtualProtect(pdh, mi.RegionSize, PAGE_READWRITE, 
		    &dwOldProtect) == FALSE)
    {
      printf("VirtualProtect: %d\n", GetLastError());
    }

  VirtualQuery(pdh, &mi, sizeof(mi));
  
  printf("Base:    0x%x\n", mi.AllocationBase);
  // this has normally PAGE_NOACCESS (0x1)
  printf("Protect: 0x%x\n", mi.AllocationProtect);

  printf("Magic:   0x%x\n", pdh->e_magic); 

  return 0;
}

int
test_env()
{
  char buf[512];

  printf("HOME=%s\n", xcegetenv("HOME"));
  printf("EMACSLOADPATH=%s\n", xcegetenv("EMACSLOADPATH"));
  printf("EMACSDATA=%s\n", xcegetenv("EMACSDATA"));

  XCEGetEnvironmentVariableFromRegA("SHELL", buf, sizeof(buf));
  printf("SHELL=%s\n", buf);

  return 0;
}

int
test_args()
{
  int i;

  for(i = 0; i < __argc; i++)
    {
      printf("Arg %d: %s\n", i, __argv[i]);
    }

  return 0;
}

int
test_filemapping(int create)
{
  HANDLE hFile;
  HANDLE hMap;
  void *file_base;
  DWORD dwCreateFlags;
  int size = 1024*1024;
  char *filename = "\\Speicherkarte2\\tmp\\filemap.dat";

  if(create)
    {
      printf("Testing file mapping (create)...\n");
      dwCreateFlags = CREATE_ALWAYS;
    }
  else
    {
      printf("Testing file mapping (read)...\n");
      dwCreateFlags = OPEN_EXISTING;
    }

  // CreateFile does not work! Error 6!
  if((hFile = XCECreateFileForMappingA(filename, 
				       GENERIC_READ|GENERIC_WRITE,
				       0, NULL,
				       dwCreateFlags, 
				       FILE_ATTRIBUTE_NORMAL, NULL))
     == INVALID_HANDLE_VALUE)
    {
      printf("CreateFileForMapping: %d\n", GetLastError());
      return -1;
    }

  if((hMap = XCECreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, size, NULL))
     == NULL)
    {
      printf("CreateFileMapping: %d\n", GetLastError());
      return -1;
    }

  if((file_base = MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 0, size))
     == NULL)
    {
      printf("MapViewOfFile: %d\n", GetLastError());
      return -1;
    }

  if(create)
    strcpy(file_base, "Hallo FileMapping!");
  else
    printf("%s\n", file_base);

  UnmapViewOfFile(file_base);

  CloseHandle(hMap);
  CloseHandle(hFile);

  printf("Testing file mapping...ok\n");

  return 0;
}

int
test_time()
{
  time_t now;
  struct _timeb tbuf;

  time(&now);
  printf("time: %.24s\n", ctime(&now));

  ftime(&tbuf);
  printf("ftime: %.24s %d\n", ctime(&tbuf.time), tbuf.millitm);
  return 0;
}

int
test_math()
{
  char *s;
  double d;

  s = ".6";
  d = atof(s);

  printf("Double is %f\n", d);
}

int
test_io()
{
  int fd;
  char buf[512];
  char *fname;

  fname = "test.txt";

  if((fd = xceopen(fname, O_WRONLY|O_CREAT|O_TRUNC, 0666)) < 0)
    {
      fprintf(stderr, "open: %d errno %d", GetLastError(), errno);
      return -1;
    }

  sprintf(buf, "%s", "Hallo\n");
  xcewrite(fd, buf, strlen(buf));
  xceclose(fd);

  return 0;
}

int
test_stat()
{
  char *fname;
  struct xcestat st;

  fname = "test.txt";
  // system does not handle . and ..
  fname = "\\Speicherkarte2\\.";
  fname = "\\Speicherkarte2\\..";
  fname = "\\Speicherkarte2";
  fname = ".";
  fname = "\\Speicherkarte2\\";

  if(xcestat(fname, &st) < 0)
    {
      fprintf(stderr, "stat: winerror: %d errno: %d\n", 
	      GetLastError(), errno);
      return -1;
    }

  printf("fname: %s\n", fname);
  printf("mode:  0x%x\n", st.st_mode);
  printf("size:  %d\n", st.st_size);
  printf("atime: %.24s\n", ctime(&st.st_atime));
  printf("ctime: %.24s\n", ctime(&st.st_ctime));
  printf("mtime: %.24s\n", ctime(&st.st_mtime));

  return 0;
}

int
test_findfile()
{
  char *fname;
  WIN32_FIND_DATAA fdataa;
  WIN32_FIND_DATAW fdataw;
  HANDLE hFind;

  if((hFind = XCEFindFirstFileA("\\*", &fdataa)) == INVALID_HANDLE_VALUE)
    {
      printf("FindFirstFile: %d\n", GetLastError());
    }
  else
    {
      do {
	printf("%s\n", fdataa.cFileName);
      } while(XCEFindNextFileA(hFind, &fdataa));

      FindClose(hFind);
    }
}

int
main(int argc, char **argv)
{
  char buf[512];

  // Wow, using stdio opens a window! Did not know that!
  fprintf(stderr, "*** CELib Tests ***\n\n");

  //test_args();
  //test_env();
  //test_image();
  test_stat();
  //test_dir();
  //test_curdir();
  //test_time();
  //test_process();
  //test_filemapping(1);
  //test_filemapping(0);
  //test_coredll();
  
  //show_virtual_memory();
  //dump_virtual_memory(0x01fb0400);
  //dump_coredll_exports();

  //test_stack2();
  //test_size();
  //test_intresource();
  //test_null();

 done:
  // prevent console from closing...
  printf("Hit RETURN");
  fflush(stdout);
  fgets(buf, sizeof(buf), stdin);

  return 0;
}
