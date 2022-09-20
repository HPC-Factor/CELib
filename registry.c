// registry.c
//
// Time-stamp: <06/01/01 11:40:17 keuchel@lightning>

#include "celib.h"

LONG 
XCERegOpenKeyExA(HKEY hKey,         
		 const char *subkey,  
		 DWORD ulOptions,   
		 REGSAM samDesired,
		 PHKEY phkResult
		 )
{
  long res;
  wchar_t subkeyw[256];
  
  MultiByteToWideChar(CP_ACP, 0, subkey, -1, subkeyw, COUNTOF(subkeyw));

  res = RegOpenKeyExW(hKey, subkeyw, ulOptions, samDesired, phkResult);

  return res;
}

LONG 
XCERegQueryValueExA(  
		    HKEY hKey,           
		    const char *valname,  
		    LPDWORD lpReserved,  
		    LPDWORD lpType,      
		    LPBYTE lpData,       
		    LPDWORD lpcbData     
		    )
{
  wchar_t valnamew[256];
  LONG res;
  LPBYTE lpDataNew = NULL;
  DWORD dwDataSize;

  dwDataSize = *lpcbData * 2;

  if(lpData != NULL)
    lpDataNew = malloc(dwDataSize);

  MultiByteToWideChar(CP_ACP, 0, valname, -1, valnamew, COUNTOF(valnamew));

  res = RegQueryValueExW(hKey, valnamew, lpReserved, lpType, lpDataNew,
			 &dwDataSize);

  if(res != 0)
    {
      free(lpDataNew);
      return res;
    }

  if(lpData)
    {
      if(*lpType == REG_SZ)
	{
	  WideCharToMultiByte(CP_ACP, 0, 
			      (wchar_t *)lpDataNew, *lpcbData, 
			      (char *) lpData, *lpcbData, 
			      NULL, NULL);
	  *lpcbData = dwDataSize/2;
	}
      else
	{
	  *lpcbData = dwDataSize;
	  memcpy(lpData, lpDataNew, *lpcbData);
	}
    }

  free(lpDataNew);

  return 0;
}

LONG 
XCERegSetValueExA(  
		  HKEY hKey,           
		  const char *valname,  
		  DWORD dwReserved,  
		  DWORD dwType,      
		  LPBYTE lpData,       
		  DWORD dwSize
		  )
{
  wchar_t valnamew[256];
  LONG res;
  LPBYTE lpDataNew = NULL;
  DWORD dwDataSize = dwSize;

  MultiByteToWideChar(CP_ACP, 0, valname, -1, valnamew, COUNTOF(valnamew));

  if(dwType == REG_SZ)
    {
      dwDataSize = dwSize * 2;
      lpDataNew = malloc(dwDataSize);

      MultiByteToWideChar(CP_ACP, 0, lpData, -1, (wchar_t *)lpDataNew, 
			  dwDataSize);
    }

  res = RegSetValueExW(hKey, valnamew, dwReserved, dwType, lpDataNew,
		       dwDataSize);

  free(lpDataNew);

  return res;
}

LONG 
XCERegEnumValueA(
		 HKEY hKey,              
		 DWORD dwIndex,          
		 char *lpValueName,     
		 LPDWORD lpcbValueName,  
		 LPDWORD lpReserved,     
		 LPDWORD lpType,         
		 LPBYTE lpData,          
		 LPDWORD lpcbData        
		 )
{
  long res;
  DWORD dwValueSize;
  DWORD dwDataSize;
  wchar_t *lpValueNameW = NULL;
  LPBYTE lpDataNew = NULL;

  dwValueSize = *lpcbValueName * 2;
  dwDataSize = *lpcbData * 2;

  if(lpValueName != NULL)
    lpValueNameW = malloc(dwValueSize);

  if(lpData != NULL)
    lpDataNew = malloc(dwDataSize);
  
  res = RegEnumValueW(hKey, dwIndex, lpValueNameW, &dwValueSize,
		      lpReserved, lpType, lpDataNew, &dwDataSize);

  if(res == 0)
    {
      if(lpValueName)
	{
	  WideCharToMultiByte(CP_ACP, 0, 
			      (wchar_t *)lpValueNameW, *lpcbValueName, 
			      (char *) lpValueName, *lpcbValueName, 
			      NULL, NULL);
	  *lpcbValueName = dwValueSize/2;
	}

      if(lpData)
	{
	  if(*lpType == REG_SZ)
	    {
	      WideCharToMultiByte(CP_ACP, 0, 
				  (wchar_t *)lpDataNew, *lpcbData, 
				  (char *) lpData, *lpcbData, 
				  NULL, NULL);
	      *lpcbData = dwDataSize/2;
	    }
	  else
	    {
	      *lpcbData = dwDataSize;
	      memcpy(lpData, lpDataNew, *lpcbData);
	    }
	}
    }

  free(lpValueNameW);
  free(lpDataNew);

  return res;
}
