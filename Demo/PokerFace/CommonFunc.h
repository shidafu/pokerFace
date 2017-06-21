#pragma once
#include <string>
using namespace std;


//名称:CCommonFunc
//功能:通过类静态函数的方式定义常用操作函数
class CCommonFunc
{
public:
	//描述: 获取当前进程的应用程序完整路径和文件名称
	//参数: wchar_t* strAppPath 读取到的文件名
	//size_t strLen 用于保存路径和文件名的缓冲区长度
	static string GetAppPathName();

	//描述: 获取当前进程的应用程序所在路径
	//参数: wchar_t* strAppPath 读取到的路径
	//size_t strLen 用于保存路径名的缓冲区长度
	static string GetAppPath();
};