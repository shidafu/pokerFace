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
}
#endif