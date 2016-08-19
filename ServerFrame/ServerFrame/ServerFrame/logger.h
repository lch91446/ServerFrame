#pragma once
#pragma warning(disable:4996)
/************************************************************************/
/*
* 文件名称：logger.h
* 摘    要：此文件实现了普通WINDOWS程序中的日志功能
*           主要有以下特点：;
*           1. 根据日期创建日志文件目录，每天的日志分别存放在不同的日志目录中;
*           2. 日志内容分三种类型，根据不同需要，写不同的日志类型的日志文件，;
*              方便通过日志定位、分析问题;
*           3. 函数经过比较好的封装，便于复用;
*           待改进点：;
*           1. 没有考虑跨平台情景，目前只使用于WINDOWS下 
*           
*/                                                                    
/************************************************************************/
#include "stdafx.h"
#include <time.h>
//#include <memory.h>
#include <stdio.h>
#include <assert.h>
//#include <stdlib.h>
//#include <stdarg.h>
#include <windows.h>
#include <fstream>
#include <iostream>
//#include <sstream>
//#include <iterator>
#include <string>

#include <boost\thread\mutex.hpp>
//#define _LOG_WRITE_STATE_ 1            /* 条件编译开关

#define LOG_SUCCESS (0)
#define LOG_FAILED (-1)

#define LOG_BOOL_TRUE (1)
#define LOG_BOOL_FALSE (0)

#define DWORD_NULL (0xFFFFFFFF)


#define MAX_LOGTEXT_LEN (2048)         /* 每行日志的最大长度*/
#define MAX_FILE_PATH (255)            /* 日志文件路径的最大长度*/
#define MAX_LOG_FILE_SIZE (512 * 1024) /* 日志文件内容的最大长度*/
#define MAX_LOG_FILE_NAME_LEN (256)    /* 日志文件名的最大长度*/
#define LOG_TYPE_INFO    0             /* 日志类型: 信息类型*/
#define LOG_TYPE_ERROR   1             /* 日志类型: 错误类型*/
#define LOG_TYPE_SYSTEM  2             /* 日志类型: 系统类型*/
#define TEST_CASE_MAX_FILE_LEN (1024)   /* 测试函数中文件内容最大长度*/
#define BEGERRLOGDATA {LogData errdata(LOG_TYPE_ERROR);\
			errdata.sFile = __FILE__;\
			errdata.sFuncName = __FUNCTION__;\
			errdata.lLineNum = __LINE__;
#define ENDERRLOGDATA }
#define  MYSTD std::
#pragma pack(push, 4)
class LogData             /* 日志内容结构体*/
{
	LogData(){};
public:
	LogData(unsigned int type)
		:iType(type),sFile(""),sFuncName(""),lLineNum(-1)
	{
		assert(0 == iType||1 == iType||2 == iType);
		time_t curTime;
		struct tm *mt;
		curTime = time(NULL);
		mt = localtime(&curTime);
		strftime(strDate, sizeof(strDate), "%Y-%m-%d", mt);
		strftime(strTime, sizeof(strTime), "%H:%M:%S", mt);
	}
	LogData(unsigned int type,MYSTD string& str)
		:iType(type),sFile(""),sFuncName(""),lLineNum(-1)
		,strText(str)
	{
		assert(0 == iType||1 == iType||2 == iType);
		time_t curTime;
		struct tm *mt;
		curTime = time(NULL);
		mt = localtime(&curTime);
		strftime(strDate, sizeof(strDate), "%Y-%m-%d", mt);
		strftime(strTime, sizeof(strTime), "%H:%M:%S", mt);
	}
	char strDate[11];   /* 日期:格式为如:2014-02-11*/;
	char strTime[9];    /* 时间:格式为如:16:10:57*/;
	unsigned int iType;         /* 日志类型:3种:INFO(0)/ERROR(1)/SYSTEM(2)*/;
	MYSTD string  sFile,sFuncName;//当前的文件 函数;
	long lLineNum; //当前的行号;
	MYSTD string strText; /*日志内容*/;
};
#pragma pack(pop)
class Logger
{
	/*Logger():fstrflg(false),scrnflg(false),scrnout(MYSTD cout)
	{
		memset(m_LogRootPath,0,sizeof(m_LogRootPath)/sizeof(m_LogRootPath[0]));
	};*/
public:
	Logger()
		:fstrflg(true),scrnflg(false),scrnout(MYSTD cout)
		,infolen(0),infonum(1),errlen(0),errnum(1),syslen(0),sysnum(1)
	{};
	Logger(MYSTD string pathName)
		:fstrflg(true),scrnflg(false),scrnout(MYSTD cout)
		,infolen(0),infonum(1),errlen(0),errnum(1),syslen(0),sysnum(1)
	{
		char pathNamebuf[MAX_FILE_PATH];
		strcpy(pathNamebuf,pathName.c_str());
		init(pathNamebuf);
	};
	Logger(char* pathName)
		:fstrflg(true),scrnflg(false),scrnout(MYSTD cout)
		,infolen(0),infonum(1),errlen(0),errnum(1),syslen(0),sysnum(1)
	{
		init(pathName);
	}
	Logger(char* pathName,MYSTD ostream & out)
		:scrnout(out),fstrflg(false),scrnflg(true)
		,infolen(0),infonum(1),errlen(0),errnum(1),syslen(0),sysnum(1)
	{
		init(pathName);
	};
	Logger(MYSTD string pathName,MYSTD ostream & out)
		:scrnout(out),fstrflg(false),scrnflg(true)
		,infolen(0),infonum(1),errlen(0),errnum(1),syslen(0),sysnum(1)
	{
		char pathNamebuf[MAX_FILE_PATH];
		strcpy(pathNamebuf,pathName.c_str());
		init(pathNamebuf);
	};
	
	void info(MYSTD string& strtext);
	void info(char * ptext,int len);
	void system(MYSTD string& strtext);
	void system(char * ptext,int len);
	void err(LogData& log_data);
	void err(MYSTD string& strtext);
	void err(char * ptext,int len);
	bool init(MYSTD string pathName);
	
	~Logger()
	{
		finfoout.flush();
		ferrout.flush();
		fsysout.flush();
		finfoout.close();
		ferrout.close();
		fsysout.close();
		
	};
private:
	void initstream();
	bool init(char* pathName)
	{
		if (LOG_SUCCESS == Create_LogDir(pathName))
		{
			strcpy(m_LogRootPath,pathName);
			initstream();
			return true;
		} 
		else
		{
			MYSTD cerr<<"日志类创建目录失败\n";
			exit(-1);
			
		}
	}
	int Create_LogDir(/*const*/ char *pStrPath);
	int Create_LogFile(/*const*/ char *pStrFile, int iPos);
	int IsFileExist(/*const */char *pStrFile);
	int GetLogFileName(int iLogType, const char *pStrPath, char *pStrName);
	int GetLogPath(char *pStrPath);
	DWORD GetFileLenth(/*const*/ char *pFile);
	int Write_Log_Text(LogData& log_data);
	inline void AddFileName(unsigned int type)
	{
		switch (type)
		{
		case LOG_TYPE_INFO:
			++infonum;
			sprintf(szInfoFileName, "%s\\app_info_%d.log", szFilePath,infonum);
			break;
		case LOG_TYPE_ERROR:
			++errnum;
			sprintf(szErrFileName, "%s\\app_error_%d.log", szFilePath,errnum);
			break;
		case LOG_TYPE_SYSTEM:
			++sysnum;
			sprintf(szSysFileName, "%s\\app_system_%d.log", szFilePath,sysnum);
			break;
		default:
			break;
		}
	}
// 	void Write_Log(LogData& log_data);
// 	void Write_Log(int type,MYSTD string& strtext);
private:
	bool fstrflg;//文件输出开关;
	bool scrnflg;//监视器输出开关;

	enum
	{
		MAX_LEN			= 10*1024*1024,//日志的最大字符数;
		MAX_FILE_SIZE	= 12*1024*1024,//最大文件size;
		LOGDATA_SIZE	= sizeof(LogData),
		MAX_FLUSH_SIZE	= 128
	};

	MYSTD ostream& scrnout;//屏幕输出引用;
	MYSTD fstream fstrout;//文件输出对象;
	MYSTD fstream finfoout;//info文件输出对象;
	MYSTD fstream ferrout;//info文件输出对象;
	MYSTD fstream fsysout;//info文件输出对象;

	char m_LogRootPath[MAX_FILE_PATH];//日志根目录;
	char szFilePath[MAX_FILE_PATH];//当前文件路径;
	char szInfoFileName[MAX_LOG_FILE_NAME_LEN];
	char szErrFileName[MAX_LOG_FILE_NAME_LEN];
	char szSysFileName[MAX_LOG_FILE_NAME_LEN];

	int infolen;
	int syslen;
	int errlen;
	int infonum;
	int errnum;
	int sysnum;

	boost::mutex mu;
};
