// cewin32.c
//
// Time-stamp: <31/01/01 21:18:53 keuchel@lightning>

#include "celib.h"

BOOL
XCEGetFileInformationByHandle(HANDLE hFile, 
			      LPBY_HANDLE_FILE_INFORMATION lpFileInformation)
{
  return GetFileInformationByHandle(hFile, lpFileInformation);
}

BOOL 
XCEGetUserNameA(char *ubuf, LPDWORD lpdwSize)
{
  char buf[126];

  if(XCEGetEnvironmentVariableFromRegA("USERNAME", buf, sizeof(buf)) == 0)
    return FALSE;

  if(ubuf != NULL)
    {
      strncpy(ubuf, buf, *lpdwSize);
    }

  *lpdwSize = strlen(buf);

  return TRUE;
}

BOOL 
XCEGetComputerNameA(char *ubuf, LPDWORD lpdwSize)
{
  char buf[126];

  if(XCEGetEnvironmentVariableFromRegA("COMPUTERNAME", buf, sizeof(buf)) == 0)
    return FALSE;

  if(ubuf != NULL)
    {
      strncpy(ubuf, buf, *lpdwSize);
    }

  *lpdwSize = strlen(buf);

  return TRUE;
}

BOOL 
XCEGetCharABCWidthsA(HDC hdc, UINT nFirst, UINT nLast, LPABC lpABC)
{
  int i;

  for(i = nFirst; i <= nLast; i++)
    {
      lpABC[i - nFirst].abcA = 0;
      lpABC[i - nFirst].abcB = 10;
      lpABC[i - nFirst].abcC = 0;
    }

  return TRUE;
}

BOOL 
XCEGetCharABCWidthsW(HDC hdc, UINT nFirst, UINT nLast, LPABC lpABC)
{
  int i;

  for(i = nFirst; i <= nLast; i++)
    {
      lpABC[i - nFirst].abcA = 0;
      lpABC[i - nFirst].abcB = 10;
      lpABC[i - nFirst].abcC = 0;
    }

  return TRUE;
}

BOOL
XCEGetVersionExA(LPOSVERSIONINFOA lpv)
{
  OSVERSIONINFOW vw;
  
  GetVersionExW(&vw);
  
  memcpy(lpv, &vw, 5 * sizeof(DWORD));

  WideCharToMultiByte(CP_ACP, 0, 
		      vw.szCSDVersion, -1, 
		      lpv->szCSDVersion, sizeof(lpv->szCSDVersion), 
		      NULL, NULL);

  return TRUE;
}

LRESULT 
XCESendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  wchar_t *lpNewText = NULL;
  LRESULT res;

  // there are lots more...

  if(Msg == WM_SETTEXT || Msg == EM_REPLACESEL)
    {
      char *s = (char *) lParam;
      int len = strlen(s) + 1;

      lpNewText = malloc(len * 2);

      MultiByteToWideChar(CP_ACP, 0, s, -1, lpNewText, len);

      lParam = (LPARAM) lpNewText;
    }
  else if(Msg == WM_GETTEXT)
    {
      lpNewText = malloc(wParam * 2);
      lParam = (LPARAM) lpNewText;
    }

  res = SendMessageW(hWnd, Msg, wParam, lParam);

  if(Msg == WM_GETTEXT)
    {
      char *s = (char *) lParam;

      WideCharToMultiByte(CP_ACP, 0, 
			  lpNewText, wParam, 
			  s, wParam, NULL, NULL);
    }

  if(lpNewText)
    free(lpNewText);

  return res;
}

// there is rarely text posted...

LRESULT 
XCEPostMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  return PostMessageW(hWnd, Msg, wParam, lParam);
}

BOOL
XCEPostThreadMessageA(DWORD idThread, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  return PostThreadMessageW(idThread, Msg, wParam, lParam);
}

BOOL 
XCEPeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax,
		UINT wRemoveMsg)
{
  return PeekMessageW(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, 
		      wRemoveMsg);
}

BOOL 
XCEGetMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
{
  return GetMessageW(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);
}

LONG 
XCEDispatchMessageA(CONST MSG *lpmsg)
{
  return DispatchMessageW(lpmsg);
}

HANDLE
XCECreateEventA(LPSECURITY_ATTRIBUTES lpEventAttributes,
		BOOL bManualReset,  
		BOOL bInitialState, 
		const char *lpName
		)
{
  HANDLE hEvent;
  wchar_t *lpNameNew = NULL;
  int len;

  if(lpName)
    {
      len = strlen(lpName) + 1;
      lpNameNew = malloc(len * 2);

      MultiByteToWideChar(CP_ACP, 0, lpName, -1, lpNameNew, len);
    }

  hEvent = CreateEventW(lpEventAttributes, bManualReset,
			bInitialState, lpNameNew);

  free(lpNameNew);

  return hEvent;
}

// does it work?
DWORD 
XCEGetModuleFileNameA(HMODULE hModule, LPTSTR lpName, DWORD nSize)
{
  wchar_t *lpNameNew = NULL;
  DWORD dwRes;

  lpNameNew = malloc(nSize * 2);
  dwRes = GetModuleFileNameW(hModule, lpNameNew, nSize);
  WideCharToMultiByte(CP_ACP, 0, 
		      lpNameNew, nSize, 
		      lpName, nSize, 
		      NULL, NULL);
  free(lpNameNew);
  return dwRes;
}

BOOL
XCESetWindowTextA(HWND hWnd, const char *s)
{
  wchar_t *lpNewText = NULL;
  BOOL res;
  int len = strlen(s) + 1;

  lpNewText = malloc(len * 2);

  MultiByteToWideChar(CP_ACP, 0, s, -1, lpNewText, len);

  res = SetWindowTextW(hWnd, lpNewText);

  free(lpNewText);

  return res;
}

int
XCEMessageBoxA(HWND hwnd, const char *txt, const char *caption, UINT flags)
{
  int res;
  wchar_t *txtw;
  wchar_t *captionw;
  int txtlen = strlen(txt) + 1;
  int captionlen = strlen(caption) + 1;
  
  txtw = malloc(txtlen * 2);
  captionw = malloc(captionlen * 2);
  
  MultiByteToWideChar(CP_ACP, 0, txt, -1, txtw, txtlen);
  MultiByteToWideChar(CP_ACP, 0, caption, -1, captionw, captionlen);

  res = MessageBoxW(hwnd, txtw, captionw, flags);

  free(txtw);
  free(captionw);

  return res;
}

BOOL 
XCEExtTextOutA(HDC hdc,   
	       int X,     
	       int Y,     
	       UINT fuOptions,
	       CONST RECT *lprc,
	       const char *txt,
	       UINT cbCount,    
	       CONST INT *lpDx  
	       )
{
  BOOL res;
  wchar_t txtw[512];

  if(cbCount > COUNTOF(txtw))
    xceabort();

  MultiByteToWideChar(CP_ACP, 0, txt, cbCount, txtw, COUNTOF(txtw));
  res = ExtTextOutW(hdc, X, Y, fuOptions, lprc, txtw, cbCount, lpDx);

  return res;
}

BOOL 
XCEAttachThreadInput(DWORD idAttach, 
		     DWORD idAttachTo,
		     BOOL fAttach)
{
  return FALSE;
}

void
XCEGetSystemTimeAsFileTime(LPFILETIME pft)
{
  SYSTEMTIME st;

  GetSystemTime(&st);
  SystemTimeToFileTime(&st, pft);
}

int 
XCEGetScrollPos(HWND hWnd, int nBar)
{
  SCROLLINFO si;

  si.cbSize = sizeof(si);
  si.fMask = SIF_ALL;

  GetScrollInfo(hWnd, nBar, &si);

  return si.nPos;
}

int 
XCESetScrollPos(HWND hWnd, int nBar, int nPos, BOOL bRedraw)
{
  SCROLLINFO si;

  si.cbSize = sizeof(si);
  si.fMask = SIF_POS;
  si.nPos = nPos;

  return SetScrollInfo(hWnd, nBar, &si, bRedraw);
}

BOOL 
XCEAdjustWindowRect(LPRECT lpRect, DWORD dwStyle, BOOL bMenu)
{
  return AdjustWindowRectEx(lpRect, dwStyle, bMenu, 0);
}

UINT 
XCEMapVirtualKeyA(UINT uCode, UINT uMapType)
{
  return MapVirtualKeyW(uCode, uMapType);
}

HFONT 
XCECreateFontIndirectA(CONST LOGFONTA *lplf)
{
  LOGFONTW lfw;
  HFONT hFont;

  lfw.lfHeight = lplf->lfHeight;
  lfw.lfWidth = lplf->lfWidth;
  lfw.lfEscapement = lplf->lfEscapement;
  lfw.lfOrientation = lplf->lfOrientation;
  lfw.lfWeight = lplf->lfWeight;
  lfw.lfItalic = lplf->lfItalic;
  lfw.lfUnderline = lplf->lfUnderline;
  lfw.lfStrikeOut = lplf->lfStrikeOut;
  lfw.lfCharSet = lplf->lfCharSet;
  lfw.lfOutPrecision = lplf->lfOutPrecision;
  lfw.lfClipPrecision = lplf->lfClipPrecision;
  lfw.lfQuality = lplf->lfQuality;
  lfw.lfPitchAndFamily = lplf->lfPitchAndFamily;

  MultiByteToWideChar(CP_ACP, 0, 
		      lplf->lfFaceName, -1, 
		      lfw.lfFaceName, COUNTOF(lfw.lfFaceName));

  hFont = CreateFontIndirectW(&lfw);

  return hFont;
}

BOOL 
XCEGetTextExtentExPointA(HDC hdc,
			 const char *s, 
			 int cchString,   
			 int nMaxExtent,  
			 LPINT lpnFit,                       
			 LPINT alpDx,     
			 LPSIZE lpSize    
			 )
{
  wchar_t *ws = NULL;
  BOOL res;

  ws = malloc(cchString * 2);

  MultiByteToWideChar(CP_ACP, 0, 
		      s, cchString, 
		      ws, cchString);
  
  res = GetTextExtentExPointW(hdc, ws, cchString, nMaxExtent, lpnFit,
			      alpDx, lpSize);

  free(ws);
  return res;
}

// When object is a HFONT, buffer contains a LOGFONT!
// This the main use in emacs (menufont).

int 
XCEGetObjectA(HGDIOBJ hgdiobj,
	      int cbBuffer,     
	      LPVOID lpvObject)
{
  if(cbBuffer == sizeof(LOGFONTA))
    {
      LOGFONTW lfw;
      LOGFONTA *plfa = (LOGFONTA *) lpvObject;
      int res;
      
      res = GetObjectW(hgdiobj, sizeof(LOGFONTW), &lfw);

      WideCharToMultiByte(CP_ACP, 0, 
			  lfw.lfFaceName, -1, 
			  plfa->lfFaceName, 
			  COUNTOF(plfa->lfFaceName), 
			  NULL, NULL);

      return res;
    }

  return GetObjectW(hgdiobj, cbBuffer, lpvObject);
}

BOOL 
XCEGetTextMetricsA(HDC hdc, LPTEXTMETRIC lptma)
{
  TEXTMETRICW tmw;
  BOOL res;

  res = GetTextMetricsW(hdc, &tmw);

  lptma->tmHeight = tmw.tmHeight;
  lptma->tmAscent = tmw.tmAscent;
  lptma->tmDescent = tmw.tmDescent;
  lptma->tmInternalLeading = tmw.tmInternalLeading;
  lptma->tmExternalLeading = tmw.tmExternalLeading;
  lptma->tmAveCharWidth = tmw.tmAveCharWidth;
  lptma->tmMaxCharWidth = tmw.tmMaxCharWidth;
  lptma->tmWeight = tmw.tmWeight;
  lptma->tmOverhang = tmw.tmOverhang;
  lptma->tmDigitizedAspectX = tmw.tmDigitizedAspectX;
  lptma->tmDigitizedAspectY = tmw.tmDigitizedAspectY;
  lptma->tmFirstChar = tmw.tmFirstChar;
  lptma->tmLastChar = tmw.tmLastChar;
  lptma->tmDefaultChar = tmw.tmDefaultChar;
  lptma->tmBreakChar = tmw.tmBreakChar;
  lptma->tmItalic = tmw.tmItalic;
  lptma->tmUnderlined = tmw.tmUnderlined;
  lptma->tmStruckOut = tmw.tmStruckOut;
  lptma->tmPitchAndFamily = tmw.tmPitchAndFamily;
  lptma->tmCharSet = tmw.tmCharSet;

  return res;
}

BOOL 
XCEAppendMenuA(HMENU hMenu,
	       UINT uFlags,
	       UINT uIDNewItem, 
	       const char *s
	       )
{
  BOOL res;
  int len;
  wchar_t *ws = NULL;

  if(s != NULL)
    {
      len = strlen(s) + 1;
      ws = malloc(len * 2);

      MultiByteToWideChar(CP_ACP, 0, 
			  s, -1, 
			  ws, len);
    }

  res = AppendMenuW(hMenu, uFlags, uIDNewItem, ws);

  if(ws)
    free(ws);

  return res;
}

HWND 
XCECreateWindowExA(DWORD dwExStyle,      
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
		)
{
  wchar_t classnamew[126];
  wchar_t windownamew[126];
  HWND hwnd;

  MultiByteToWideChar(CP_ACP, 0, 
		      lpClassName, -1, 
		      classnamew, COUNTOF(classnamew));

  MultiByteToWideChar(CP_ACP, 0, 
		      lpWindowName, -1, 
		      windownamew, COUNTOF(windownamew));

  hwnd = CreateWindowExW(dwExStyle, classnamew, windownamew, dwStyle,
			 x, y, nWidth, nHeight, hWndParent, hMenu,
			 hInstance, lpParam);

  return hwnd;
}

HMODULE 
XCEGetModuleHandleA(const char *lpName)
{
  HMODULE hMod;
  wchar_t *lpNameNew = NULL;
  int len;

  if(lpName)
    {
      len = strlen(lpName) + 1;
      lpNameNew = malloc(len * 2);

      MultiByteToWideChar(CP_ACP, 0, lpName, -1, lpNameNew, len);
    }

  hMod = GetModuleHandleW(lpNameNew);

  free(lpNameNew);

  return hMod;
}

HANDLE 
XCECreateFileMappingA(HANDLE hFile, 
		      LPSECURITY_ATTRIBUTES lpFileMappingAttributes, 
		      DWORD flProtect, 
		      DWORD dwMaximumSizeHigh, 
		      DWORD dwMaximumSizeLow, 
		      const char *lpName)
{
  HANDLE hFileMap;
  wchar_t *lpNameNew = NULL;
  int len;

  if(lpName)
    {
      len = strlen(lpName) + 1;
      lpNameNew = malloc(len * 2);

      MultiByteToWideChar(CP_ACP, 0, lpName, -1, lpNameNew, len);
    }

  hFileMap = CreateFileMappingW(hFile, lpFileMappingAttributes,
				flProtect, dwMaximumSizeHigh,
				dwMaximumSizeLow, lpNameNew);

  free(lpNameNew);

  return hFileMap;
}

const char *
XCEGetCommandLineA()
{
  return _commandline;
}

int
XCEGetEnvironmentVariableA(const char *name, char *buf, int len)
{
  char *p;

  if(buf)
    buf[0] = 0;

  if((p = xcegetenv(name)) == NULL)
    return 0;

  if(buf == NULL)
    return strlen(p);

  strcpy(buf, p);

  return strlen(buf);
}

int
XCEGetEnvironmentVariableFromRegA(const char *name, char *buf, int len)
{
  int res;
  char data[1024];
  HKEY hKey;
  DWORD dwDataSize = sizeof(data);
  DWORD dwType;

  if(buf)
    buf[0] = 0;

  if((res = XCERegOpenKeyExA(HKEY_LOCAL_MACHINE, "Environment", 0,
			     KEY_READ, &hKey)) != 0)
    {
      return 0;
    }

  res = XCERegQueryValueExA(hKey, name, NULL, &dwType, 
			 data, &dwDataSize);

  XCERegCloseKey(hKey);

  if(res != 0 || dwType != REG_SZ)
    return 0;

  if(buf == NULL)
    return strlen(data);

  strncpy(buf, data, len);

  return strlen(buf);
}

BOOL
XCESetEnvironmentVariableInRegA(const char *name, const char *value)
{
  int res;
  HKEY hKey;
  
  if((res = XCERegOpenKeyExA(HKEY_LOCAL_MACHINE, "Environment", 0,
			     KEY_READ, &hKey)) != 0)
    {
      return FALSE;
    }

  res = XCERegSetValueExA(hKey, name, 0, REG_SZ, 
			  (LPBYTE) value, strlen(value) + 1);

  XCERegCloseKey(hKey);

  return res == 0;
}

HMODULE
XCELoadLibraryA(const char *name)
{
  wchar_t namew[MAX_PATH];

  MultiByteToWideChar(CP_ACP, 0, name, -1, namew, COUNTOF(namew));

  return LoadLibraryW(namew);
}

FARPROC
XCEGetProcAddressA(HMODULE hMod, const char *name)
{
  wchar_t namew[126];

  MultiByteToWideChar(CP_ACP, 0, name, -1, namew, COUNTOF(namew));

  return GetProcAddressW(hMod, namew);
}

BOOL
XCEGetDiskFreeSpaceExA(const char *path, 
		       ULARGE_INTEGER *pl1, 
		       ULARGE_INTEGER *pl2, 
		       ULARGE_INTEGER *pl3)
{
  wchar_t pathw[MAX_PATH];

  MultiByteToWideChar(CP_ACP, 0, path, -1, pathw, COUNTOF(pathw));

  return GetDiskFreeSpaceExW(pathw, pl1, pl2, pl3);
}

		     
