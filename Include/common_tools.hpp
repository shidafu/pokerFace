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
	@fn  path_to_jsonpath
	@brief path to jsonpath.
	@param[in] jsonpath: full path.
	@param[out] path: dir path, delete filename and ext, include '\' at last.
	@param[in] pathMaxLen: max length of path.
	*/
	void path_to_jsonpath(const char* path, char* jsonpath,  int pathMaxLen)
	{
		int fullLen = strlen(path);
		int pathLen = fullLen;
		int j = 0;
		for (int i=0; i<fullLen; i++)
		{
			if (path[i] == '\\' || path[i] == '/')
			{
				jsonpath[j] = '\\';
				j++;
				if (j >= pathMaxLen) break;
				jsonpath[j] = '\\';
				j++;
				if (j >= pathMaxLen) break;
			}
			else
			{
				jsonpath[j] = path[i];
				j++;
				if (j >= pathMaxLen) break;
			}
		}
		if (j >= pathMaxLen) return;
		jsonpath[j] = 0;
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
		files.clear();
		dir.append(ext);
		intptr_t hFile;
		_finddata_t fileinfo;
		if ((hFile = _findfirst(dir.c_str(), &fileinfo)) != -1)
		{
			do
			{
				if (!(fileinfo.attrib & _A_SUBDIR))//ур╣╫нд╪Ч  
				{
					files.push_back(fileinfo.name);
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
		return files.size();
	}
}
#endif