/**
@file common_tools.hpp
@brief just some tool
@author Leon Contact: towanglei@163.com
@version 1.0
@par History:
      1.0 : Leon 2017/05/11 15:54 build
*/
#pragma once
#ifndef COMMON_TOOLS
#define COMMON_TOOLS
#include <string>  
#include <direct.h>  
#include <io.h> 
namespace tools
{
	/**
	@fn  full_to_path
	@brief full path to path.
	@param[in] full: full path.
	@param[out] path: dir path, delete filename and ext, include '\' at last.
	@param[in] pathMaxLen: max length of path.
	*/
	void full_to_path(const char* full, char* path, int pathMaxLen)
	{
		int fullLen = strlen(full);
		int pathLen = fullLen;
		for (; pathLen >= 0; pathLen--)
		{
			if (full[pathLen - 1] == '\\' || full[pathLen - 1] == '/') break;
		}
		if (pathLen < pathMaxLen)
		{
			for (int i = 0; i < pathLen; i++)
			{
				path[i] = full[i];
			}
			path[pathLen] = 0;
		}
		return;
	}

	/**
	@fn  full_to_path
	@brief full path to path.
	@param[in] fullstr: full path.
	@return: dir path, delete filename and ext, include '\' at last.
	*/
	std::string full_to_path(std::string fullstr)
	{
		const char* full = fullstr.c_str();
		char path[1024];
		int fullLen = strlen(full);
		int pathLen = fullLen;
		for (; pathLen >= 0; pathLen--)
		{
			if (full[pathLen - 1] == '\\' || full[pathLen - 1] == '/') break;
		}
		if (pathLen < 1024)
		{
			for (int i = 0; i < pathLen; i++)
			{
				path[i] = full[i];
			}
			path[pathLen] = 0;
		}
		std::string pathStr = path;
		return pathStr;
	}
	
	/**
	@fn  app_path
	@brief full get app path.
	*/
	std::string app_path()
	{
#ifdef _WINDOWS
		char strTempAppPath[512] = "\0";
		memset(strTempAppPath, 0, 512);
		::GetModuleFileNameA(NULL, strTempAppPath, 512);

		char* pChar = strTempAppPath;
		while (*pChar != '\0')
		{
			pChar++;
		}
		while (*pChar != '\\')
		{
			pChar--;
		}
		*pChar = '\0';
		std::string strAppPath = strTempAppPath;
		return strAppPath;
#else
		return "";
#endif
	}

	/**
	@fn  dir_files
	@brief get files in dir.
	*/
	int dir_files(std::string dir, std::string ext, std::vector<std::string>& files)
	{
//#ifdef _WINDOWS
//		files.clear();
//		_WIN32_FIND_DATAA FindFileData;
//		HANDLE hFind;
//		hFind = ::FindFirstFileA((dir + "*.*").c_str(), (LPWIN32_FIND_DATAA)&FindFileData);
//		if (hFind != INVALID_HANDLE_VALUE)
//		{
//			do
//			{
//				if ((FindFileData.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
//				{
//					std::string name = FindFileData.cFileName;
//					int iLength = name.length();
//					if (iLength > 1)
//					{
//						int lastLen = 0;
//						char* pEndChar = (char*)FindFileData.cFileName + iLength;
//						while (*pEndChar != L'.')
//						{
//							pEndChar--;
//							lastLen++;
//						}
//						pEndChar++;
//						supr(pEndChar, lastLen);
//						if (wcscmp(pEndChar, strUppExt) == 0)
//						{
//							Set.iItemNum++;
//							if (Set.iItemNum >= Set.MAX_ITEM_NUM)
//							{
//								break;
//							}
//						}
//					}
//				}
//			} while (FindNextFile(hFind, &FindFileData));
//			FindClose(hFind);
//		}
//		return files.size();
//#else
		return 0;
//#endif
	}
}
#endif