#include "CommonFunc.h"
#include <windows.h>

string CCommonFunc::GetAppPathName()
{
	char strTempAppPath[512] = "\0";
	memset(strTempAppPath, 0, 512);
	::GetModuleFileNameA(NULL, strTempAppPath, 512);
	string strAppPath = strTempAppPath;
	return strAppPath;
}

string CCommonFunc::GetAppPath()
{
	char strTempAppPath[512] = "\0";
	memset(strTempAppPath, 0, 512);
	::GetModuleFileNameA(NULL, strTempAppPath, 512);

	char* pChar = strTempAppPath;
	while(*pChar != '\0')
	{
		pChar++;
	}
    while(*pChar != '\\')
	{
		pChar--;
	}
	*pChar = '\0';
	string strAppPath = strTempAppPath;
	return strAppPath;
}