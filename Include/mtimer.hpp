/**
@file mtimer.hpp
@brief Time counter
@note
	Usage:\n
	<pre>
	{
		mtimer watch;
		Do something...
			...
	}>> wsprintf timeUsed
	double timeStoreHere;
	{
		mtimer watch;
		watch.SetData(&timeStoreHere);
		Do something...
			...
	}>> wsprintf timeUsed
	Get timeStoreHere;
	</pre>
@author Leon Contact: towanglei@163.com
@copyright Leon
@version 1.0
@par History:
	1.0 : Leon 2014/05/01 19:03 Build.
*/

#pragma once
#ifndef mtimer
#define mtimer
#include <windows.h>
#include <iostream>
#include "mlog.hpp"

#ifdef _DEBUG
#ifdef _CONSOLE
#ifndef DEBUG_CMD
#define DEBUG_CMD
#endif
#endif
#endif

//namespace tools
//{
	/**
	@class MThread
	@brief Time counter
	@note
		Usage:\n
		<pre>
		{
			mtimer watch;
			Do something...
				...
		}>> wsprintf timeUsed
		double timeStoreHere;
		{
			mtimer watch;
			watch.SetData(&timeStoreHere);
			Do something...
				...
		}>> wsprintf timeUsed
		Get timeStoreHere;
		</pre>
	@author Leon Contact: towanglei@163.com
	@version 3.0
	@date 2016/04/29 22:34
	*/
	class mtimer
	{
	private:
		LARGE_INTEGER m_liStart, m_liEnd, m_liFrequency;
		double m_dCycle;
		double*  m_pTime;
	public:
		void SetData(double* pTime)
		{
			m_pTime = pTime;
		}
		mtimer()
		{
			QueryPerformanceFrequency(&m_liFrequency);
			m_dCycle = 1000 * 1.0 / m_liFrequency.QuadPart;
			QueryPerformanceCounter(&m_liStart);
			m_pTime = NULL;
		}
		~mtimer()
		{
			QueryPerformanceCounter(&m_liEnd);
			double dTime = m_dCycle * (m_liEnd.QuadPart - m_liStart.QuadPart);
			if (NULL != m_pTime)
			{
				*m_pTime = dTime;
			}
			//#ifdef DEBUG_CMD
			//	std::cout<<"<mtimer::"<<(int)dTime<<"ms>"<<std::endl;
			//#endif
			MLOG << "<mtimer::" << (int)dTime << "ms>\n";
		}
	};
//}
#endif