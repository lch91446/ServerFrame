// ServerFrame.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


App theapp;//Ψһʵ��;
int _tmain(int argc, _TCHAR* argv[])
{
	if (theapp.initialize())
	{
		std::cout<<"��ʼ�����\n";
		std::cout<<"calc server....\n"<<std::endl;
		theapp.run();
	}
	else
	{
		std::cout<<"��ʼ��ʧ��\n";
	}
	return 0;
}
