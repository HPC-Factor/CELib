#ifndef CELIB_H
#define CELIB_H 1

#include <stdlib.h>
#include <windows.h>
#include "sys/types.h"
#include "errno.h"

#define MAXFDS 100

#define MAX_USERNAME_LENGTH 126
#define MAX_COMPUTERNAME_LENGTH 126

#define R_OK 1
#define W_OK 2
#define X_OK 4

typedef struct _fdent
{
  int fd;
  int type;
  HANDLE hFile;
} _fdent_t;

extern _fdent_t _fdtab[];

#define XCE_FILE_TYPE_FILE   1
#define XCE_FILE_TYPE_SOCKET 2

#undef _T
#define _T(X) L##X

#define COUNTOF(X) (sizeof(X)/sizeof(X[0]))

#ifdef __cplusplus
extern "C" {
#endif

// gdi, needed for emacs

typedef struct _ABC {
    int     abcA;
    UINT    abcB;
    int     abcC;
} ABC, *PABC, NEAR *NPABC, FAR *LPABC;

// ctype.h
#define isdigit(C) iswdigit(C)
#define isalpha(C) iswalpha(C)
#define islower(C) iswlower(C)
#define isupper(C) iswupper(C)
#define isspace(C) iswspace(C)
#define isalnum(C) iswalnum(C)
#define iscntrl(C) iswcntrl(C)
#define isprint(C) iswprint(C)
#define ispunct(C) iswpunct(C)
#define isxdigit(C) iswxdigit(C)
#define isascii(C) iswascii(C)

// stat.h
#define _S_IFMT         0170000         /* file type mask */
#define _S_IFDIR        0040000         /* directory */
#define _S_IFCHR        0020000         /* character special */
#define _S_IFIFO        0010000         /* pipe */
#define _S_IFREG        0100000         /* regular */
#define _S_IREAD        0000400         /* read permission, owner */
#define _S_IWRITE       0000200         /* write permission, owner */
#define _S_IEXEC        0000100         /* execute/search permission, owner */

#define S_IFMT   _S_IFMT
#define S_IFREG  _S_IFREG
#define S_IFDIR  _S_IFDIR
#define S_IREAD  _S_IREAD
#define S_IWRITE _S_IWRITE
#define S_IEXEC  _S_IEXEC

#ifndef S_ISDIR
#define S_ISDIR(X) (((X) & S_IFMT) == S_IFDIR)
#endif
#ifndef S_ISREG
#define S_ISREG(X) (((X) & S_IFMT) == S_IFREG)
#endif

// fcntl.h
#define _O_RDONLY       0x0000  /* open for reading only */
#define _O_WRONLY       0x0001  /* open for writing only */
#define _O_RDWR         0x0002  /* open for reading and writing */
#define _O_APPEND       0x0008  /* writes done at eof */
#define _O_CREAT        0x0100  /* create and open file */
#define _O_TRUNC        0x0200  /* open and truncate */
#define _O_EXCL         0x0400  /* open only if file doesn't already exist */
#define _O_TEXT         0x4000  /* file mode is text (translated) */
#define _O_BINARY       0x8000  /* file mode is binary (untranslated) */

#define _O_NOINHERIT    0

#define O_RDONLY        _O_RDONLY
#define O_WRONLY        _O_WRONLY
#define O_RDWR          _O_RDWR
#define O_APPEND        _O_APPEND
#define O_CREAT         _O_CREAT
#define O_TRUNC         _O_TRUNC
#define O_EXCL          _O_EXCL
#define O_TEXT          _O_TEXT
#define O_BINARY        _O_BINARY

// stdio
#define _IOFBF          0x0000
#define _IOLBF          0x0040
#define _IONBF          0x0004

#define BUFSIZ          512

//////////////////////////////////////////////////////////////////////

extern int _fmode;
extern char **environ;
extern int __argc;
extern char **__argv;
extern char *_commandline;

void xceinit(const wchar_t *commandlinew);

//////////////////////////////////////////////////////////////////////

int xceisatty(int fd);

int xcesnprintf(char *buf, size_t count, const char *fmt, ...);
int xceftruncate(int fd, unsigned long size);
HANDLE xceget_osfhandle(int fd);

int utime (const char *fname, struct _utimbuf *times);
int futime (int fh, struct _utimbuf *times);
void ftime (struct _timeb *tp);

FILE *xcefopen(const char *path, const char *mode);
FILE *xcefreopen(const char *path, const char *mode, FILE *stream);
void xcefclose();
FILE *xcefdopen(int fd, const char *mode);
int xcefileno(FILE *f);
FILE *xcepopen(const char *command, const char *mode);
int xcepclose(FILE *f);

int xcegetpid();

char *xcestrrchr(const char * string, int ch);

int xcecreat(const char *filename, int pmode);
int xceopen(const char *fname, int mode, ...);
int xceopenw(const wchar_t *fname, int mode, ...);
int xceread(int fd, void *buf, int size);
int xcewrite(int fd, void *buf, int size);
int xceclose(int fd);
off_t xcelseek(int fd, int off, int whence);

int xcestat(const char *fname, struct xcestat *st);
int xcefstat(int fd, struct xcestat *st);

char *xcegetenv(const char *name);
int xceputenv(const char *s);

void xceabort();

int xcechdir(const char *dirname);
int xcechroot(const char *dirname);
char *xcegetcwd(char *buf, int size);
int xcesystem(const char *cmd);

void xcerewind(FILE *f);

int xcelocking(int fd, int mode, int nbytes);

char *xcestrerror(int n);
void xceperror(const char *s);

int xceaccess(const char *fname, int mode);
int xceunlink(const char *fname);

void (*signal(int, void (*)(int)))(int);
int xceraise(int);

char *xcestrdup(const char * string);
int xcestricmp(const char * dst, const char * src);
int xcestrcmpi(const char * dst, const char * src);
int xcestrnicmp(const char * first, const char * last, size_t count);
char *xcestrupr(char *string);
char *xcestrlwr(char *string);
size_t xcestrspn(const char *string, const char *control);
size_t xcestrcspn(const char *string, const char *control);

char *strpbrk(const char *string, const char *control);

double xcestrtod(const char *s, const char **errorptr);
long xcestrtol(const char *s, const char **errorptr, int base);
unsigned long xcestrtoul(const char *s, const char **errorptr, int base);

int xceumask(int mask);

int xcemkdir(const char *dirname);
int xcermdir(const char *dirname);
int xcerename(const char *old, const char *new);
int xcechmod(const char *fname, int mode);

struct servent *xcegetservbyname(const char *sname, const char *sproto);

//////////////////////////////////////////////////////////////////////

BOOL XCEGetDiskFreeSpaceExA(const char *path, 
			    ULARGE_INTEGER *pl1, 
			    ULARGE_INTEGER *pl2, 
			    ULARGE_INTEGER *pl3);

const char *XCEGetCommandLineA();
int XCEGetEnvironmentVariableA(const char *name, char *buf, int len);

int XCEGetEnvironmentVariableFromRegA(const char *name, char *buf, int len);
BOOL XCESetEnvironmentVariableInRegA(const char *name, const char *value);

int XCEGetScrollPos(HWND hWnd, int nBar);
int XCESetScrollPos(HWND hWnd, int nBar, int nPos, BOOL bRedraw);

DWORD XCEGetCurrentDirectoryA(DWORD dwSize, char *buf);
DWORD XCEGetCurrentDirectoryW(DWORD dwSize, wchar_t *buf);
BOOL XCESetCurrentDirectoryA(const char *dir);
BOOL XCESetCurrentDirectoryW(const wchar_t *wdir);

DWORD XCEGetCurrentRootDirectoryA(DWORD dwSize, char *buf);
DWORD XCEGetCurrentRootDirectoryW(DWORD dwSize, wchar_t *buf);
BOOL XCESetCurrentRootDirectoryA(const char *dir);
BOOL XCESetCurrentRootDirectoryW(const wchar_t *wdir);

DWORD XCEGetFileAttributesA(const char *fname);
DWORD XCEGetFileAttributesW(const wchar_t *fname);

BOOL XCESetFileAttributesA(const char *fname, DWORD dwAttr);
BOOL XCESetFileAttributesW(const wchar_t *fname, DWORD dwAttr);

HMODULE XCELoadLibraryA(const char *name);
FARPROC XCEGetProcAddressA(HMODULE hMod, const char *name);

HANDLE XCECreateFileA(
		      LPCSTR fname,
		      DWORD dwDesiredAccess,
		      DWORD dwShareMode,
		      LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		      DWORD dwCreationDisposition,
		      DWORD dwFlagsAndAttributes,
		      HANDLE hTemplateFile
		      );

HANDLE XCECreateFileW(
		      LPCWSTR wfname,
		      DWORD dwDesiredAccess,
		      DWORD dwShareMode,
		      LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		      DWORD dwCreationDisposition,
		      DWORD dwFlagsAndAttributes,
		      HANDLE hTemplateFile
		      );

HANDLE XCECreateFileForMappingA(
				LPCSTR fname,
				DWORD dwDesiredAccess,
				DWORD dwShareMode,
				LPSECURITY_ATTRIBUTES lpSecurityAttributes,
				DWORD dwCreationDisposition,
				DWORD dwFlagsAndAttributes,
				HANDLE hTemplateFile
				);


HANDLE XCECreateFileForMappingW(
				LPCWSTR fname,
				DWORD dwDesiredAccess,
				DWORD dwShareMode,
				LPSECURITY_ATTRIBUTES lpSecurityAttributes,
				DWORD dwCreationDisposition,
				DWORD dwFlagsAndAttributes,
				HANDLE hTemplateFile
				);

BOOL XCEWriteFile(
		  HANDLE hFile,
		  LPCVOID lpBuffer,
		  DWORD nNumberOfBytesToWrite,
		  LPDWORD lpNumberOfBytesWritten,
		  LPOVERLAPPED lpOverlapped
		  );

BOOL XCEReadFile(
		 HANDLE hFile,
		 LPVOID lpBuffer,
		 DWORD nNumberOfBytesToRead,
		 LPDWORD lpNumberOfBytesRead,
		 LPOVERLAPPED lpOverlapped
		 );

BOOL XCEGetFileInformationByHandle(HANDLE hFile, 
				   LPBY_HANDLE_FILE_INFORMATION lpFileInfo);

BOOL XCEGetUserName(char *buf, LPDWORD lpdwSize);
BOOL XCEGetComputerName(char *buf, LPDWORD lpdwSize);

// returns static string
char *XCEGetUnixPath(const char *path);

BOOL XCEDeleteFileA(const char *fname);
BOOL XCEDeleteFileW(const wchar_t *fname);

BOOL XCEGetCharABCWidthsA(HDC hdc, UINT nFirst, UINT nLast, LPABC lpABC);
BOOL XCEGetCharABCWidthsW(HDC hdc, UINT nFirst, UINT nLast, LPABC lpABC);

LONG XCERegOpenKeyExA(HKEY hKey,         
		      const char *subkey,  
		      DWORD ulOptions,   
		      REGSAM samDesired,
		      PHKEY phkResult
		      );

LONG XCERegQueryValueExA(  
			 HKEY hKey,           
			 const char *valname,  
			 LPDWORD lpReserved,  
			 LPDWORD lpType,      
			 LPBYTE lpData,       
			 LPDWORD lpcbData     
			 );

LONG XCERegSetValueExA(  
		       HKEY hKey,           
		       const char *valname,  
		       DWORD dwReserved,  
		       DWORD dwType,      
		       LPBYTE lpData,       
		       DWORD dwSize
		       );

LONG XCERegEnumValueA(
		      HKEY hKey,              
		      DWORD dwIndex,          
		      char *lpValueName,     
		      LPDWORD lpcbValueName,  
		      LPDWORD lpReserved,     
		      LPDWORD lpType,         
		      LPBYTE lpData,          
		      LPDWORD lpcbData        
		      );

BOOL XCEGetVersionExA(LPOSVERSIONINFOA lpv);

LRESULT XCESendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT XCEPostMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL XCEPostThreadMessageA(DWORD idThread, UINT Msg, WPARAM, LPARAM);
BOOL XCEPeekMessageA(LPMSG lpMsg, HWND hWnd, UINT, UINT, UINT wRemoveMsg);
BOOL XCEGetMessageA(LPMSG lpMsg, HWND hWnd, UINT, UINT);
LONG XCEDispatchMessageA(CONST MSG *lpmsg);

HANDLE XCECreateEventA(LPSECURITY_ATTRIBUTES lpEventAttributes,
		       BOOL bManualReset,  
		       BOOL bInitialState, 
		       const char *lpName
		       );

DWORD XCEGetModuleFileNameA(HMODULE hModule, LPTSTR lpName, DWORD nSize);

HANDLE XCEFindFirstFileA(const char *lpName, LPWIN32_FIND_DATAA lpfd);
HANDLE XCEFindFirstFileW(const wchar_t *oldpath, LPWIN32_FIND_DATAW lpfd);

BOOL XCEFindNextFileA(HANDLE hFind, LPWIN32_FIND_DATAA lpfd);
BOOL XCEFindNextFileW(HANDLE hFind, LPWIN32_FIND_DATAW lpfd);

BOOL XCECreateDirectoryA(const char *lpName, LPSECURITY_ATTRIBUTES);
BOOL XCECreateDirectoryW(const wchar_t *lpName, LPSECURITY_ATTRIBUTES);

BOOL XCERemoveDirectoryA(const char *lpName);
BOOL XCERemoveDirectoryW(const wchar_t *lpName);

BOOL XCEMoveFileA(const char *lpName1, const char *lpName2);
BOOL XCEMoveFileW(const wchar_t *lpName1, const wchar_t *lpName2);

BOOL XCECopyFileA(const char *lpName1, const char *lpName2, BOOL);
BOOL XCECopyFileW(const wchar_t *lpName1, const wchar_t *lpName2, BOOL);

BOOL XCESetWindowTextA(HWND hWnd, const char *s);

int XCEMessageBoxA(HWND hwnd, const char *txt, const char *caption, UINT);

BOOL XCEExtTextOutA(HDC hdc,   
		    int X,     
		    int Y,     
		    UINT fuOptions,
		    CONST RECT *lprc,
		    const char *txt,
		    UINT cbCount,    
		    CONST INT *lpDx  
		    );

BOOL XCEAttachThreadInput(DWORD idAttach, 
			  DWORD idAttachTo,
			  BOOL fAttach);

BOOL XCEAdjustWindowRect(LPRECT lpRect, DWORD dwStyle, BOOL bMenu);

UINT XCEMapVirtualKeyA(UINT uCode, UINT uMapType);

HFONT XCECreateFontIndirectA(CONST LOGFONT *lplf);

BOOL XCEGetTextExtentExPointA(HDC hdc,
			      const char *s, 
			      int cchString,   
			      int nMaxExtent,  
			      LPINT lpnFit,                       
			      LPINT alpDx,     
			      LPSIZE lpSize    
			      );

int XCEGetObjectA(HGDIOBJ hgdiobj, int cbBuffer, LPVOID lpvObject);
BOOL XCEGetTextMetricsA(HDC hdc, LPTEXTMETRIC lptm);

BOOL XCEAppendMenuA(HMENU hMenu,
		    UINT uFlags,
		    UINT uIDNewItem, 
		    const char *s
		    );

HWND XCECreateWindowExA(DWORD dwExStyle,      
			LPCTSTR lpClassName,  
			LPCTSTR lpWindowName, 
			DWORD dwStyle,        
			int x,                
			int y,                
			int nWidth,           
			int nHeight,          
			HWND hWndParent,      
			HMENU hMenu,          
			HINSTANCE hInstance,  
			LPVOID lpParam
			);

HMODULE XCEGetModuleHandleA(const char *lpName);

HANDLE XCECreateFileMappingA(HANDLE hFile, 
			     LPSECURITY_ATTRIBUTES lpFileMappingAttributes, 
			     DWORD flProtect, 
			     DWORD dwMaximumSizeHigh, 
			     DWORD dwMaximumSizeLow, 
			     const char *lpName);
  
//////////////////////////////////////////////////////////////////////
// utils

void XCEShowMessageA(const char *fmt, ...);
void XCEShowMessageW(const wchar_t *fmt, ...);

int XCESearchPathA(const char *pathlist, const char *file, char *retpath);
int XCEExecuteProcessA(const char *command, BOOL bWait, LPDWORD lpdwProcId);
int XCEWaitProcess(DWORD dwProcId);

void XCEShowWaitCursor();
void XCEShowNormalCursor();

//////////////////////////////////////////////////////////////////////

#define XCERegCloseKey RegCloseKey
#define XCECloseHandle CloseHandle

#ifndef BUILD_CELIB

#define strrchr xcestrrchr
#define _strrchr xcestrrchr
#define strdup xcestrdup
#define _strdup xcestrdup
#define strnicmp xcestrnicmp
#define _strnicmp xcestrnicmp
#define stricmp xcestricmp
#define _stricmp xcestricmp
#define strcmpi xcestrcmpi
#define _strcmpi xcestrcmpi
#define strlwr xcestrlwr
#define _strlwr xcestrlwr
#define strupr xcestrupr
#define _strupr xcestrupr

#define strtod xcestrtod
#define _strtod xcestrtod
#define strtol xcestrtol
#define _strtol xcestrtol
#define strtoul xcestrtoul
#define _strtoul xcestrtoul

#ifndef chdir
#define chdir xcechdir
#endif
#define _chdir xcechdir

#ifndef open
#define open xceopen
#endif
#define _open xceopen
#ifndef read
#define read xceread
#endif
#define _read xceread
#ifndef write
#define write xcewrite
#endif
#define _write xcewrite
#ifndef close
#define close xceclose
#endif
#define _close xceclose

#define stat xcestat
#define _stat xcestat
#define fstat xcefstat
#define _fstat xcefstat

#ifndef lseek
#define lseek xcelseek
#endif
#define _lseek xcelseek
#ifndef creat
#define creat xcecreat
#endif
#define _creat xcecreat

#ifndef popen
#define popen xcepopen
#endif
#define _popen xcepopen
#ifndef pclose
#define pclose xcepclose
#endif
#define _pclose xcepclose

#define perror xceperror
#ifndef strerror
#define strerror xcestrerror
#define _strerror xcestrerror
#endif
#ifndef abort
#define abort xceabort
#define _abort xceabort
#endif
#define getcwd xcegetcwd

#define getenv xcegetenv
#define putenv xceputenv
#define _getenv xcegetenv
#define _putenv xceputenv

#define system xcesystem
#define rewind xcerewind

#define isatty xceisatty
#define setmode xcesetmode
#define _isatty xceisatty
#define _setmode xcesetmode

#define _locking xcelocking
#define locking xcelocking

#ifndef access
#define access xceaccess
#endif
#define _access xceaccess

#ifndef unlink
#define unlink xceunlink
#endif
#define _unlink xceunlink

#ifndef signal
#define signal xcesignal
#endif
#define _signal xcesignal

#undef CreateFile
#define CreateFile XCECreateFileA
#undef ReadFile
#define ReadFile XCEReadFile
#undef WriteFile
#define WriteFile XCEWriteFile
#undef CloseHandle
#define CloseHandle XCECloseHandle
#undef GetFileAttributes
#define GetFileAttributes XCEGetFileAttributesA

#define GetCurrentDirectory XCEGetCurrentDirectoryA
#define SetCurrentDirectory XCESetCurrentDirectoryA

#define GetUserName XCEGetUserNameA
#define GetComputerName XCEGetComputerNameA

#undef DeleteFile
#define DeleteFile XCEDeleteFileA

#define GetCharABCWidthsA XCEGetCharABCWidthsA
#define GetCharABCWidthsW XCEGetCharABCWidthsW
#define GetCharABCWidths XCEGetCharABCWidthsA

#define GetVersionExA XCEGetVersionExA

#define RegOpenKeyExA XCERegOpenKeyExA
#define RegQueryValueExA XCERegQueryValueExA
#define RegEnumValueA XCERegEnumValueA

#define SendMessageA XCESendMessageA
#define PostMessageA XCEPostMessageA
#define PostThreadMessageA XCEPostThreadMessageA
#define PeekMessageA XCEPeekMessageA
#define GetMessageA XCEGetMessageA
#define DispatchMessageA XCEDispatchMessageA

#define CreateEventA XCECreateEventA

#define GetModuleFileNameA XCEGetModuleFileNameA

#define FindFirstFileA XCEFindFirstFileA
#define FindNextFileA XCEFindNextFileA

#define SetWindowTextA XCESetWindowTextA
#define MessageBoxA XCEMessageBoxA

#define ExtTextOutA XCEExtTextOutA

#define AttachThreadInput XCEAttachThreadInput

#define GetScrollPos XCEGetScrollPos
#define SetScrollPos XCESetScrollPos

#define AdjustWindowRect XCEAdjustWindowRect

#define MapVirtualKeyA XCEMapVirtualKeyA

#define CreateFontIndirectA XCECreateFontIndirectA
#define GetTextExtentExPointA XCEGetTextExtentExPointA
#define GetObjectA XCEGetObjectA
#define GetTextMetricsA XCEGetTextMetricsA
#define AppendMenuA XCEAppendMenuA
#define CreateWindowExA XCECreateWindowExA

#define GetModuleHandleA XCEGetModuleHandleA
#define CreateFileMappingA XCECreateFileMappingA

#define GetCommandLine XCEGetCommandLineA
#define GetCommandLineA XCEGetCommandLineA
#define GetEnvironmentVariable XCEGetEnvironmentVariableA
#define GetEnvironmentVariableA XCEGetEnvironmentVariableA
#define GetEnvironmentVariableFromReg XCEGetEnvironmentVariableFromRegA
#define GetEnvironmentVariableFromRegA XCEGetEnvironmentVariableFromRegA

#define CreateFileForMappingA XCECreateFileForMappingA

#define LoadLibraryA XCELoadLibraryA
#define GetProcAddressA XCEGetProcAddressA

// stdlib.h
#define alloca _alloca

#ifndef umask
#define umask xceumask
#endif
#define _umask xceumask

#ifndef mkdir
#define mkdir xcemkdir
#endif
#define _mkdir xcemkdir
#ifndef rmdir
#define rmdir xcermdir
#endif
#define _rmdir xcermdir
#ifndef rename
#define rename xcerename
#endif
#define _rename xcerename
#ifndef chmod
#define chmod xcechmod
#endif
#define _chmod xcechmod

#ifndef getpid
#define getpid xcegetpid
#endif
#define _getpid xcegetpid

// time funcs
#define tzset _tzset
#define tzname _tzname

#define snprintf xcesnprintf
#define ftruncate xceftruncate
#define get_osfhandle xceget_osfhandle
#define _get_osfhandle xceget_osfhandle

#define wsstart xcewsstart
#define wsstop xcewsstop
#define wserror xcewserror
#define wsstrerror xcewsstrerror
#define winstrerror xcewinstrerror

#define rcmd xcercmd

#define fopen xcefopen
#define freopen xcefreopen
#define fclose xcefclose

#ifndef fdopen
#define fdopen xcefdopen
#endif

#ifndef fileno
#define fileno xcefileno
#endif

#define _ftime ftime

#define chroot xcechroot

#endif

#ifdef XCEDEBUG
#define XCETRACE(X) XCETrace X
#define XCETRACEW(X) XCETraceW X
#else
#define XCETRACE(X)
#define XCETRACEW(X)
#endif

void XCETrace(const char *fmt, ...);
void XCETraceW(const wchar_t *fmt, ...);

#ifdef __cplusplus
};
#endif

#endif
