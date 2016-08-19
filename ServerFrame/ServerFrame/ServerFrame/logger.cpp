#include "stdafx.h"
#include "logger.h"

#define _LOG_WRITE_STATE_  1        /* 条件编译开关，



/*********************************************************************
* 函数名称:int GetLogPath(char *pStrPath)
* 说明:获取日志文件路径
* 调用者：Write_Log_Text
* 输入参数:
* 无
* 输出参数：
* char *pStrPath
* 返回值:
* int -- LOG_FAILED: 失败
*      -- LOG_SUCCESS: 成功
*********************************************************************/
int Logger::GetLogPath(char *pStrPath)
{
	if(NULL == pStrPath)
	{
		return LOG_FAILED;
	}
	int iRet = 0;
	time_t curTime = time(NULL);
	struct tm *mt = localtime(&curTime);
	/* 根据日期组成文件夹名称*/
	sprintf(pStrPath, "%s\\%d%02d%02d", m_LogRootPath, mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday);
	iRet = Create_LogDir(pStrPath);

	return iRet;
}

/*********************************************************************
* 函数名称:int GetLogFileName(int iLogType, const char *pStrPath, char *pStrName)
* 说明:获取日志文件名
* 调用者：Write_Log_Text
* 输入参数:
* int iLogType         -- 日志类型 3种:INFO(0)/ERROR(1)/SYSTEM(2)
* const char *pStrPath -- 日志路径 由GetLogPath得到
* 输出参数：
* char *pStrName       -- 日志文件名
* 返回值:
* int -- LOG_FAILED: 失败
*      -- LOG_SUCCESS: 成功
*********************************************************************/
int Logger::GetLogFileName(int iLogType, const char *pStrPath, char *pStrName)
{
	if(NULL == pStrPath)
	{
		return LOG_FAILED;
	}
	char szLogName[MAX_FILE_PATH];
	/*FILE *pFile = NULL;*/
	memset(szLogName, 0, MAX_FILE_PATH);
	switch (iLogType)
	{
	case LOG_TYPE_INFO:
		sprintf(szLogName, "%s\\app_info_%d.log", pStrPath,infonum);
		break;
	case LOG_TYPE_ERROR:
		sprintf(szLogName, "%s\\app_error_%d.log", pStrPath,errnum);
		break;
	case LOG_TYPE_SYSTEM:
		sprintf(szLogName, "%s\\app_system_%d.log", pStrPath,sysnum);
		break;
	default:
		return LOG_FAILED;
		break;
	}
	//strcat(szLogName, ".log");
	while(IsFileExist(szLogName))
	{
		/* 如果文件长度大于指定的最大长度，重新创建一文件*/;
// 		if (((int)GetFileLenth(szLogName) + 256) >= MAX_FILE_SIZE)
// 			{
		switch (iLogType)
		{
		case LOG_TYPE_INFO:
			++infonum;
			sprintf(szLogName, "%s\\app_info_%d.log", pStrPath,infonum);
			break;
		case LOG_TYPE_ERROR:
			++errnum;
			sprintf(szLogName, "%s\\app_error_%d.log", pStrPath,errnum);
			break;
		case LOG_TYPE_SYSTEM:
			++sysnum;
			sprintf(szLogName, "%s\\app_system_%d.log", pStrPath,sysnum);
			break;
		default:
			return LOG_FAILED;
			break;
		}
		continue;
//			}
// 			else
// 			{
// 				Create_LogFile(szLogName, 0);
// 				break;
// 			}
	}
	Create_LogFile(szLogName, 0);
	sprintf(pStrName, "%s", szLogName);
	return LOG_SUCCESS;
}

/*********************************************************************
* 函数名称:int Create_LogDir(const char *pStrPath)
* 说明:创建日志存放路径
* 调用者：GetLogPath
* 输入参数:
* const char *pStrPath --用户指定的根路径
* 输出参数：
* 无
* 返回值:
* int -- LOG_FAILED: 失败
*      -- LOG_SUCCESS: 成功
*********************************************************************/
int Logger::Create_LogDir(/*const*/ char *pStrPath)
{
	if(NULL == pStrPath)
	{
		return LOG_FAILED;
	}
	int iRet = 0;
	char szSub[MAX_FILE_PATH];
	char *pSub = NULL;
	//int iIndex = 0;
	int iLen = 0;
	int bFind = 0;
	memset(szSub, 0, MAX_FILE_PATH);
	/* 逐层创建目录*/
	while(1)
	{
		pSub = strchr(pStrPath + iLen, '\\');
		if(NULL == pSub)
		{
			if(iLen == 0)
			{
				return LOG_FAILED;
			}
			iRet = CreateDirectory(pStrPath, NULL);
			if(0 == iRet)
			{
				iRet = GetLastError();
				if(ERROR_ALREADY_EXISTS == iRet)
				{
					return LOG_SUCCESS;
				}
				return LOG_FAILED;
			}
			return LOG_SUCCESS;
		}
		else
		{
			if (!bFind)
			{
				bFind = 1;
			}
			else
			{
				memset(szSub, 0, sizeof(szSub));
				strncpy(szSub, pStrPath, pSub - pStrPath);
				CreateDirectory(szSub, NULL);
			}
			iLen = pSub - pStrPath + 1;
		}
	}
	return LOG_SUCCESS;
}

/*********************************************************************
* 函数名称:int Create_LogFile(const char *pStrFile, int iPos)
* 说明:创建日志文件
* 调用者：GetLogFileName
* 输入参数:
* const char *pStrFile --文件名
* int iPos             --文件指针位置
* 输出参数：
* 无
* 返回值:
* int -- LOG_FAILED: 失败
*      -- LOG_SUCCESS: 成功
*********************************************************************/
int Logger::Create_LogFile(/*const*/ char *pStrFile, int iPos)
{
	HANDLE hd = 0;
	int iRet = 0;
	if(NULL == pStrFile)
	{
		return LOG_FAILED;
	}
	hd = CreateFile(pStrFile,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if(INVALID_HANDLE_VALUE == hd)
	{
		return LOG_FAILED;
	}
	if(DWORD_NULL == SetFilePointer(hd, iPos, NULL, FILE_BEGIN))
	{
		return LOG_FAILED;
	}
	iRet = SetEndOfFile(hd);
	CloseHandle(hd);
	return iRet;
}

/*********************************************************************
* 函数名称:int IsFileExist(const char *pStrFile)
* 说明:判断指定的文件是否存在
* 调用者：GetLogFileName
* 输入参数:
* const char *pStrFile --文件名
* 输出参数：
* 无
* 返回值:
* int -- LOG_BOOL_FALSE: 不存在
*      -- LOG_BOOL_TRUE: 存在
*********************************************************************/
int Logger::IsFileExist(/*const*/ char *pStrFile)
{
	//int iLen = 0;
	WIN32_FIND_DATA finddata;
	memset(&finddata, 0, sizeof(WIN32_FIND_DATA));
	HANDLE hd = FindFirstFile(pStrFile, &finddata);
	if(INVALID_HANDLE_VALUE == hd)
	{
		DWORD dwRet = GetLastError();
		if(ERROR_FILE_NOT_FOUND == dwRet || ERROR_PATH_NOT_FOUND == dwRet)
		{
			return LOG_BOOL_FALSE;
		}
	}
	FindClose(hd);
	return LOG_BOOL_TRUE;
}

/*********************************************************************
* 函数名称:DWORD GetFileLenth(const char *pFile)
* 说明:判断指定的文件大小
* 调用者：GetLogFileName
* 输入参数:
* const char *pFile --文件名
* 输出参数：
* 无
* 返回值:
* DWORD -- 文件大小
*********************************************************************/
DWORD Logger::GetFileLenth(/*const*/ char *pFile)
{
	WIN32_FIND_DATA buff;
	HANDLE hd = NULL;
	memset(&buff, 0, sizeof(WIN32_FIND_DATA));
	hd = FindFirstFile(pFile, &buff);
	FindClose(hd);
	return (buff.nFileSizeHigh * MAXDWORD) + buff.nFileSizeLow;
}

/*********************************************************************
* 函数名称:int Write_Log_Text(LogData& log_data)
* 说明:写日志内容
* 调用者：Write_Log
* 输入参数:
* LPLOG_DATA lpLogData --日志内容结构体量
* 输出参数：
* 无
* 返回值:
* int -- LOG_FAILED: 失败
*      -- LOG_SUCCESS: 成功
*********************************************************************/
int Logger::Write_Log_Text( LogData& log_data )
{
	//typedef boost:: unique_lock OnlyOneLock;
	
	/*if (scrnflg)
	{
		boost::mutex::scoped_lock lock(mu);
		switch(log_data.iType)
		{
		case LOG_TYPE_ERROR:
			  {
				  
				  scrnout
					  <<"错误信息 "
					  <<"文件名 "<<log_data.sFile<<" "
					  <<"函数 "<<log_data.sFuncName<<" "
					  <<"行号 "<<log_data.lLineNum<<'\n';
				  scrnout
					  <<log_data.strDate<<" "
					  <<log_data.strTime<<" "
					  <<log_data.strText<<"\n";
				  break;
			  }
		case LOG_TYPE_INFO:
			{
				scrnout
					<<"程序日志 "
					<<log_data.strDate<<" "
					<<log_data.strTime<<"   "
					<<log_data.strText<<"\n";
				break;
			}
		case LOG_TYPE_SYSTEM:
			{
				scrnout
					<<"系统日志 "
					<<log_data.strDate<<" "
					<<log_data.strTime<<"   "
					<<log_data.strText<<"\n";
				break;
			}
		}
		return LOG_SUCCESS;
	}
	else */
	if (fstrflg)
	{
		boost::mutex::scoped_lock lock(mu);
		switch(log_data.iType)
			{
			case LOG_TYPE_ERROR:
				{
					if (ferrout.good())
					{
					ferrout
						<<"错误信息 "
						<<"文件名 "<<log_data.sFile<<" "
						<<"函数 "<<log_data.sFuncName<<" "
						<<"行号 "<<log_data.lLineNum<<"\n["
						<<log_data.strDate<<"] ["
						<<log_data.strTime<<"] "
						<<log_data.strText<<"\n";
#ifdef _DEBUG
					ferrout.flush();
#endif
					}
					
					break;
				}
			case LOG_TYPE_INFO:
				{
					if(finfoout.good())
					{
					finfoout
						<<"程序日志 ["
						<<log_data.strDate<<"] ["
						<<log_data.strTime<<"] "
						<<log_data.strText<<"\n";
#ifdef _DEBUG
					finfoout.flush();
#endif
					}
					
					break;
				}	
			case LOG_TYPE_SYSTEM:
				{
					if (fsysout.good())
					{
					fsysout
						<<"系统日志 ["
						<<log_data.strDate<<"] ["
						<<log_data.strTime<<"] "
						<<log_data.strText<<"\n";
					fsysout.flush();
#ifdef _DEBUG
					fsysout.flush();
#endif
					}
					//fsysout.flush();
					break;
				}
			default:
				{
					bool tempbool = false;
					assert(tempbool);
					break;
				}
				
			}
			//fstrout.close();
			return LOG_SUCCESS;
	}	 
	else
	{
		return LOG_FAILED;
	}
	
	
}

void Logger::info( MYSTD string& strtext )
{
#if _LOG_WRITE_STATE_   /* 写日志与否的编译开关*/
	if (infolen < MAX_LEN)
	{
		infolen += strtext.length();
// 		if (infolen > MAX_FLUSH_SIZE)
// 		{
// 			finfoout.flush();
// 		}
	} 
	else
	{
		//关闭流，重新开始写入新文件;
		infolen = 0;
		infolen += strtext.length();
		finfoout.flush();
		finfoout.close();
		//GetLogFileName(LOG_TYPE_INFO,szFilePath,szInfoFileName);
		AddFileName(LOG_TYPE_INFO);
		finfoout.open(szInfoFileName,MYSTD ios::out|MYSTD ios::app);
	}
	LogData tempdata(LOG_TYPE_INFO);
	tempdata.strText = strtext;
	Write_Log_Text(tempdata);
#endif _LOG_WRITE_STATE_
}

void Logger::info( char * ptext,int len )
{
#if _LOG_WRITE_STATE_   /* 写日志与否的编译开关*/
	if (!ptext)
	{
		return;
	}
	info(MYSTD string(ptext,len));
#endif _LOG_WRITE_STATE_
}

void Logger::system( MYSTD string& strtext )
{
#if _LOG_WRITE_STATE_   /* 写日志与否的编译开关*/
	if (syslen < MAX_LEN)
	{
		syslen += strtext.length();
// 		if (syslen > MAX_FLUSH_SIZE)
// 		{
// 			fsysout.flush();
// 		}
	} 
	else
	{
		//关闭流，重新开始写入新文件;
		syslen = 0;
		syslen += strtext.length();
		fsysout.flush();
		fsysout.close();
		//GetLogFileName(LOG_TYPE_INFO,szFilePath,szSysFileName);
		AddFileName(LOG_TYPE_SYSTEM);
		fsysout.open(szSysFileName,MYSTD ios::out|MYSTD ios::app);
	}
	LogData tempdata(LOG_TYPE_SYSTEM);
	tempdata.strText = strtext;
	Write_Log_Text(tempdata);
#endif _LOG_WRITE_STATE_
}

void Logger::system( char * ptext,int len )
{
#if _LOG_WRITE_STATE_   /* 写日志与否的编译开关*/
	if (!ptext)
	{
		return;
	}
	system(MYSTD string(ptext,len));
#endif _LOG_WRITE_STATE_
}

void Logger::initstream()
{
	memset(szFilePath, 0, MAX_FILE_PATH);
	memset(szInfoFileName, 0, MAX_LOG_FILE_NAME_LEN);
	memset(szErrFileName, 0, MAX_LOG_FILE_NAME_LEN);
	memset(szSysFileName, 0, MAX_LOG_FILE_NAME_LEN);

	GetLogPath(szFilePath);

	GetLogFileName(LOG_TYPE_INFO, szFilePath, szInfoFileName);
	finfoout.open(szInfoFileName,MYSTD ios::out|MYSTD ios::app);

	GetLogFileName(LOG_TYPE_ERROR, szFilePath, szErrFileName);
	ferrout.open(szErrFileName,MYSTD ios::out|MYSTD ios::app);

	GetLogFileName(LOG_TYPE_SYSTEM, szFilePath, szSysFileName);
	fsysout.open(szSysFileName,MYSTD ios::out|MYSTD ios::app);
}

void Logger::err( LogData& log_data )
{
#if _LOG_WRITE_STATE_   /* 写日志与否的编译开关*/
	if (errlen < MAX_LEN)
	{
		errlen += LOGDATA_SIZE ;
// 		if (errlen > MAX_FLUSH_SIZE)
// 		{
// 			ferrout.flush();
// 		}
	} 
	else
	{
		//关闭流，重新开始写入新文件;
		errlen = 0;
		errlen += LOGDATA_SIZE ;;
		ferrout.flush();
		ferrout.close();
		AddFileName(LOG_TYPE_ERROR);
		ferrout.open(szErrFileName,MYSTD ios::out|MYSTD ios::app);
	}
	Write_Log_Text(log_data);
#endif _LOG_WRITE_STATE_
}

void Logger::err( MYSTD string& strtext )
{
#if _LOG_WRITE_STATE_
	if (errlen < MAX_LEN)
	{
		errlen += LOGDATA_SIZE ;
// 		if (errlen > MAX_FLUSH_SIZE)
// 		{
// 			ferrout.flush();
// 		}
	} 
	else
	{
		//关闭流，重新开始写入新文件;
		errlen = 0;
		errlen += LOGDATA_SIZE ;
		ferrout.flush();
		ferrout.close();
		AddFileName(LOG_TYPE_ERROR);
		ferrout.open(szErrFileName,MYSTD ios::out|MYSTD ios::app);
	}
	LogData tempdata(LOG_TYPE_ERROR,strtext);
	Write_Log_Text(tempdata);
#endif _LOG_WRITE_STATE_
}

void Logger::err( char * ptext,int len )
{
#if _LOG_WRITE_STATE_
	if (errlen < MAX_LEN)
	{
		errlen += LOGDATA_SIZE ;
// 		if (errlen > MAX_FLUSH_SIZE)
// 		{
// 			ferrout.flush();
// 		}
	} 
	else
	{
		//关闭流，重新开始写入新文件;
		errlen = 0;
		errlen += LOGDATA_SIZE ;
		ferrout.flush();
		ferrout.close();
		AddFileName(LOG_TYPE_ERROR);
		ferrout.open(szErrFileName,MYSTD ios::out|MYSTD ios::app);
	}
	LogData tempdata(LOG_TYPE_ERROR,MYSTD string(ptext,len));
	Write_Log_Text(tempdata);
#endif _LOG_WRITE_STATE_
}

bool Logger::init( MYSTD string pathName )
{
	char szFilePath[MAX_FILE_PATH];
	std::strcpy(szFilePath,pathName.c_str());
	return init(szFilePath);
}

//void Logger::Write_Log(LogData& log_data)
//{
//#if _LOG_WRITE_STATE_   /* 写日志与否的编译开关*/
//	Write_Log_Text(log_data);
//#endif _LOG_WRITE_STATE_
//}

//void Logger::Write_Log( int type,MYSTD string& strtext )
//{
//#if _LOG_WRITE_STATE_   /* 写日志与否的编译开关*/
//	assert(LOG_TYPE_ERROR != type);
//	LogData tempdata(type);
//	tempdata.strText = strtext;
//	Write_Log_Text(tempdata);
//#endif _LOG_WRITE_STATE_
//}