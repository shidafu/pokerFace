#pragma once
#include <string>
using namespace std;


//����:CCommonFunc
//����:ͨ���ྲ̬�����ķ�ʽ���峣�ò�������
class CCommonFunc
{
public:
	//����: ��ȡ��ǰ���̵�Ӧ�ó�������·�����ļ�����
	//����: wchar_t* strAppPath ��ȡ�����ļ���
	//size_t strLen ���ڱ���·�����ļ����Ļ���������
	static string GetAppPathName();

	//����: ��ȡ��ǰ���̵�Ӧ�ó�������·��
	//����: wchar_t* strAppPath ��ȡ����·��
	//size_t strLen ���ڱ���·�����Ļ���������
	static string GetAppPath();
};