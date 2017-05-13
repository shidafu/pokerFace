///////////////////////////////////////////////////
/**
@file mformat.hpp
@brief Convert data format.
@author Leon Contact: towanglei@163.com
@copyright Leon
@version 1.0
@History:
    1.0 : Leon 2013/02/20 17:35 build
*/
#pragma once
#ifndef MFORMAT
#define MFORMAT
#include <string>
#include <sstream>
namespace tools
{
	/** 
	@fn  CopyData
	@brief Priority use T::operator= if has(memcopy if not),when deep copy data in.
	@param[in] T& src
	@param[out] T& dst
	@return bool
	*/
	template <typename T>
	bool copy_data(T& dst,const T& src)
	{
		__if_exists(T::operator=)
		{
			dst = src;
		}
		__if_not_exists(T::operator=)
		{
			memcpy_s(&dst, sizeof(T), &src, sizeof(T));
		}
		return true;
	}

	///* 
	//Converts a character string into an int or long 
	//将一个字符串转化为整数 
	//*/
	//int matoi(char s[])  
	//{  
	//    int i,n,sign;
	//    for(i=0;isspace(s[i]);i++);   //跳过空白  
	//    sign=(s[i]=='-')?-1:1;  
	//    if(s[i]=='+'||s[i]==' -')     //跳过符号位  
	//        i++;  
	//    for(n=0;isdigit(s[i]);i++)  
	//        n=10*n+(s[i]-'0');        //将数字字符转换成整形数字  
	//    return sign*n;  
	//}  
	//  
	///* 
	//Converts an int or long into a character string 
	//将一个整数转化为字符串 
	//*/  
	//void mitoa(int n,char s[])  
	//{  
	//    int i,j,sign;  
	//  
	//    if((sign=n)<0)    //记录符号  
	//        n=-n;         //使n成为正数  
	//    i=0;  
	//    do{  
	//        s[i++]=n%10+'0';    //取下一个数字  
	//    }while((n/=10)>0);      //循环相除  
	//  
	//    if(sign<0)  
	//        s[i++]='-';  
	//    s[i]='\0';  
	//    for(j=i-1;j>=0;j--)        //生成的数字是逆序的，所以要逆序输出  
	//        printf("%c",s[j]);  
	//}

	std::string ws_to_s(const std::wstring& ws)  
	{  
		std::string curLocale = setlocale(LC_ALL, NULL);
		setlocale(LC_ALL, "chs");  
		const wchar_t* _Source = ws.c_str();  
		size_t _Dsize = 2 * ws.size() + 1;  
		char *_Dest = new char[_Dsize];  
		memset(_Dest,0,_Dsize);  
		wcstombs(_Dest,_Source,_Dsize);  
		std::string result = _Dest;  
		delete []_Dest;  
		setlocale(LC_ALL, curLocale.c_str());  
		return result;  
	}  
  
	std::wstring s_to_ws(const std::string& s)  
	{  
		setlocale(LC_ALL, "chs");   
		const char* _Source = s.c_str();  
		size_t _Dsize = s.size() + 1;  
		wchar_t *_Dest = new wchar_t[_Dsize];  
		wmemset(_Dest, 0, _Dsize);  
		mbstowcs(_Dest,_Source,_Dsize);  
		std::wstring result = _Dest;  
		delete []_Dest;  
		setlocale(LC_ALL, "C");  
		return result;  
	}

	std::string i_to_s(int data)
	{
		std::stringstream sstream; 
		sstream<<data;
		std::string str=sstream.str();
		sstream.clear();
		sstream.str("");
		return str;
	}

	std::string l_to_s(long data)
	{
		std::stringstream sstream; 
		sstream<<data;
		std::string str=sstream.str();
		sstream.clear();
		sstream.str("");
		return str;
	}

	std::string d_to_s(double data)
	{
		std::stringstream sstream; 
		sstream<<data;
		std::string str=sstream.str();
		sstream.clear();
		sstream.str("");
		return str;
	}

	std::string f_to_s(float data)
	{
		std::stringstream sstream; 
		sstream<<data;
		std::string str=sstream.str();
		sstream.clear();
		sstream.str("");
		return str;
	}
}


#endif