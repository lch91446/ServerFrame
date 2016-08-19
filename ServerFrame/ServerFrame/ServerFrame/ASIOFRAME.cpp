// ASIOFRAME.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "App.h"

App theapp;
int _tmain(int argc, _TCHAR* argv[])
{
	if (theapp.initialize())
	{
		std::cout<<"初始化完成\n";
		std::cout<<"calc server....\n"<<std::endl;
		theapp.run();
	}
	else
	{
		std::cout<<"初始化失败\n";
	}
	return 0;
}
