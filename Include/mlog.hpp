/**
@file mlog.hpp
@brief Log to several output.
@author Leon Contact: towanglei@163.com
@copyright Leon
@version 1.0
@par History:
	1.0 : Leon 2014/05/01 19:03 Build.
*/

#pragma once
#ifndef MLOG
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


#ifdef _WINDOWS
#include <Windows.h>
#include <process.h>
#define WM_MMSG  (WM_USER+1324)
#endif

namespace tools
{
#ifdef _WINDOWS
	/**
	@class mmsg
	@brief Send user windows message and store string in protected buffer, used as global object.
	@note
	<pre>
	Post message as fallow:
	mmsg mmsg;
	Postmmsg(std::string msgStr,int type = 1)
	Define in user as fallow:
	BEGIN_MESSAGE_MAP(...)
	ON_MESSAGE(WM_TEST,OnMmsg)
	END_MESSAGE_MAP()
	afx_msg void OnMmsg(WPARAM wParam,LPARAM lParam)
	{
	std::string& msgStr;
	int type = get_mmsg(msgStr);
	switch(type)
	{
	}
	}
	</pre>
	@author Leon Contact: towanglei@163.com
	@version 1.0
	@date 2017/04/24 22:34
	*/
	class mmsg
	{
	private:
		/// Message lock.
		CRITICAL_SECTION m_msgSection;
		/// Message for global use.
		std::string m_msgString;
		int m_msgType;
		int m_msgRecived;
		HWND m_hWnd;
	public:
		mmsg(HWND hWnd = 0)
		{
			m_msgString = "";
			m_msgType = 0;
			m_msgRecived = 0;
			m_hWnd = hWnd;
			::InitializeCriticalSection(&m_msgSection);
		}
		~mmsg()
		{
			::DeleteCriticalSection(&m_msgSection);
		}
		/**
		@fn  initial
		@param[in] hWnd: Handel of window.\n
		*/
		bool initial(HWND hWnd)
		{
			m_hWnd = hWnd;
			return true;
		}

		void post_mmsg(std::string msgStr, int type = 1)
		{
			::EnterCriticalSection(&m_msgSection);
			m_msgType = type;
			m_msgString = msgStr;
			m_msgRecived = 0;
			::LeaveCriticalSection(&m_msgSection);
			::PostMessage(m_hWnd, WM_MMSG, 0, 0);
		}
		int get_mmsg(std::string& msgStr)
		{
			if (m_msgRecived > 1) return -1;
			msgStr = m_msgString;
			::EnterCriticalSection(&m_msgSection);
			m_msgRecived++;
			::LeaveCriticalSection(&m_msgSection);
			return m_msgType;
		}
		//BEGIN_MESSAGE_MAP(...)
		//ON_MESSAGE(WM_TEST,OnMMsg)
		//END_MESSAGE_MAP()
		//afx_msg void OnMMsg(WPARAM wParam,LPARAM lParam)
		//{
		//	std::string msgStr;
		//	int type = GetMMsg(msgStr);
		//	switch(type)
		//	{
		//	}
		//}
	};
	/// Static MMsg for global use.
	static mmsg globalMsg;
#define MMSG_INI(h) globalMsg.initial(h)
	/// MMsg micro for global use.
#define MMSG_POSTMSG(x,t) globalMsg.post_mmsg(x,t)
/// MMsg micro for global use.
#define MMSG_GETMSG(x) globalMsg.get_mmsg(x)
#endif

/**
@class mlog
@brief Log tool.
@author Leon Contact: towanglei@163.com
@version 1.0
@date 2017/04/24 22:34
*/
	class mlog
	{
	private:
		std::string logPath;
		std::ofstream fout;
	public:
		/**
		@fn  MLog
		@param[in] filePath: Log file path if "" do not output to file.\n
		*/
		mlog(std::string filePath = "")
		{
			logPath = "";
			if (filePath.compare("") != 0)
			{
				fout.open(filePath, std::ofstream::out | std::ofstream::app);
				if (fout.is_open())
				{
					logPath = filePath;
				}
			}
		}

		~mlog()
		{
			if (fout.is_open())
			{
				fout.close();
			}
		}

		/**
		@fn  initial
		@param[in] filePath: Log file path.\n
		*/
		bool initial(std::string filePath)
		{
			if (filePath.compare("") == 0)
			{
				return false;
			}
			if (fout.is_open())
			{
				if (filePath.compare(logPath) == 0)
				{
					return true;
				}
				fout.close();
			}
			fout.open(filePath, std::ofstream::out | std::ofstream::app);
			if (fout.is_open())
			{
				logPath = filePath;
				return true;
			}
			return false;
		}

		/**
		@fn  out_std
		@param[in] logStr: Log string.\n
		*/
		void out_std(std::string logStr)
		{
#ifdef _DEBUG 
			if (fout.is_open())
			{
				fout << logStr;
			}
			//else
			{
#ifdef _CONSOLE
				std::cout << logStr;
#endif
			}
#endif
		}

		/**
		@fn  operator<<
		@brief Output for continue useage as mLog<<"abc "<<"abc "<<"\n".
		@param[in] logStr: Log string.\n
		*/
		mlog& operator<<(std::string logStr)
		{
#ifdef _DEBUG 
			if (fout.is_open())
			{
				fout << logStr;
			}
			//else
			{
#ifdef _CONSOLE
				std::cout << logStr;
#endif
			}
#ifdef _WINDOWS
			OutputDebugStringA(logStr.data());
#endif
#endif
			return *this;
		}
		/**
		@fn  operator<<
		@brief Output for continue useage as mLog<<"abc "<<"abc "<<"\n".
		@param[in] logData: Log string.\n
		*/
		mlog& operator<<(int logData)
		{
#ifdef _DEBUG 
			if (fout.is_open())
			{
				fout << logData;
			}
			//else
			{
#ifdef _CONSOLE
				std::cout << logData;
#endif
			}
#ifdef _WINDOWS
			std::stringstream sstream;
			sstream << logData;
			std::string str = sstream.str();
			OutputDebugStringA(str.data());
			sstream.clear();
			sstream.str("");
#endif
#endif
			return *this;
		}
		mlog& operator<<(double logData)
		{
#ifdef _DEBUG 
			if (fout.is_open())
			{
				fout << logData;
			}
			//else
			{
#ifdef _CONSOLE
				std::cout << logData;
#endif
			}
#ifdef _WINDOWS
			std::stringstream sstream;
			sstream << logData;
			std::string str = sstream.str();
			OutputDebugStringA(str.data());
			sstream.clear();
			sstream.str("");
#endif
#endif
			return *this;
		}
		mlog& operator<<(float logData)
		{
#ifdef _DEBUG 
			if (fout.is_open())
			{
				fout << logData;
			}
			//else
			{
#ifdef _CONSOLE
				std::cout << logData;
#endif
			}
#ifdef _WINDOWS
			std::stringstream sstream;
			sstream << logData;
			std::string str = sstream.str();
			OutputDebugStringA(str.data());
			sstream.clear();
			sstream.str("");
#endif
#endif
			return *this;
		}
	};
/// Static MLog for global use.
static mlog globalLog;
/// MLog micro for global use.
#define MLOG_INI(f) globalLog.initial(f)
/// MLog micro for global use.
#define MLOG_OUT(x) globalLog<<x
/// MLog micro for global use.
#define MLOG        globalLog
}

#endif