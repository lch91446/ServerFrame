#include "stdafx.h"
#include "logger.h"

#define _LOG_WRITE_STATE_  1        /* �������뿪�أ�



/*********************************************************************
* ��������:int GetLogPath(char *pStrPath)
* ˵��:��ȡ��־�ļ�·��
* �����ߣ�Write_Log_Text
* �������:
* ��
* ���������
* char *pStrPath
* ����ֵ:
* int -- LOG_FAILED: ʧ��
*      -- LOG_SUCCESS: �ɹ�
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
	/* ������������ļ�������*/
	sprintf(pStrPath, "%s\\%d%02d%02d", m_LogRootPath, mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday);
	iRet = Create_LogDir(pStrPath);

	return iRet;
}

/*********************************************************************
* ��������:int GetLogFileName(int iLogType, const char *pStrPath, char *pStrName)
* ˵��:��ȡ��־�ļ���
* �����ߣ�Write_Log_Text
* �������:
* int iLogType         -- ��־���� 3��:INFO(0)/ERROR(1)/SYSTEM(2)
* const char *pStrPath -- ��־·�� ��GetLogPath�õ�
* ���������
* char *pStrName       -- ��־�ļ���
* ����ֵ:
* int -- LOG_FAILED: ʧ��
*      -- LOG_SUCCESS: �ɹ�
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
		/* ����ļ����ȴ���ָ������󳤶ȣ����´���һ�ļ�*/;
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
* ��������:int Create_LogDir(const char *pStrPath)
* ˵��:������־���·��
* �����ߣ�GetLogPath
* �������:
* const char *pStrPath --�û�ָ���ĸ�·��
* ���������
* ��
* ����ֵ:
* int -- LOG_FAILED: ʧ��
*      -- LOG_SUCCESS: �ɹ�
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
	/* ��㴴��Ŀ¼*/
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
* ��������:int Create_LogFile(const char *pStrFile, int iPos)
* ˵��:������־�ļ�
* �����ߣ�GetLogFileName
* �������:
* const char *pStrFile --�ļ���
* int iPos             --�ļ�ָ��λ��
* ���������
* ��
* ����ֵ:
* int -- LOG_FAILED: ʧ��
*      -- LOG_SUCCESS: �ɹ�
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
* ��������:int IsFileExist(const char *pStrFile)
* ˵��:�ж�ָ�����ļ��Ƿ����
* �����ߣ�GetLogFileName
* �������:
* const char *pStrFile --�ļ���
* ���������
* ��
* ����ֵ:
* int -- LOG_BOOL_FALSE: ������
*      -- LOG_BOOL_TRUE: ����
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
* ��������:DWORD GetFileLenth(const char *pFile)
* ˵��:�ж�ָ�����ļ���С
* �����ߣ�GetLogFileName
* �������:
* const char *pFile --�ļ���
* ���������
* ��
* ����ֵ:
* DWORD -- �ļ���С
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
* ��������:int Write_Log_Text(LogData& log_data)
* ˵��:д��־����
* �����ߣ�Write_Log
* �������:
* LPLOG_DATA lpLogData --��־���ݽṹ����
* ���������
* ��
* ����ֵ:
* int -- LOG_FAILED: ʧ��
*      -- LOG_SUCCESS: �ɹ�
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
					  <<"������Ϣ "
					  <<"�ļ��� "<<log_data.sFile<<" "
					  <<"���� "<<log_data.sFuncName<<" "
					  <<"�к� "<<log_data.lLineNum<<'\n';
				  scrnout
					  <<log_data.strDate<<" "
					  <<log_data.strTime<<" "
					  <<log_data.strText<<"\n";
				  break;
			  }
		case LOG_TYPE_INFO:
			{
				scrnout
					<<"������־ "
					<<log_data.strDate<<" "
					<<log_data.strTime<<"   "
					<<log_data.strText<<"\n";
				break;
			}
		case LOG_TYPE_SYSTEM:
			{
				scrnout
					<<"ϵͳ��־ "
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
						<<"������Ϣ "
						<<"�ļ��� "<<log_data.sFile<<" "
						<<"���� "<<log_data.sFuncName<<" "
						<<"�к� "<<log_data.lLineNum<<"\n["
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
						<<"������־ ["
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
						<<"ϵͳ��־ ["
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
#if _LOG_WRITE_STATE_   /* д��־���ı��뿪��*/
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
		//�ر��������¿�ʼд�����ļ�;
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
#if _LOG_WRITE_STATE_   /* д��־���ı��뿪��*/
	if (!ptext)
	{
		return;
	}
	info(MYSTD string(ptext,len));
#endif _LOG_WRITE_STATE_
}

void Logger::system( MYSTD string& strtext )
{
#if _LOG_WRITE_STATE_   /* д��־���ı��뿪��*/
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
		//�ر��������¿�ʼд�����ļ�;
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
#if _LOG_WRITE_STATE_   /* д��־���ı��뿪��*/
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
#if _LOG_WRITE_STATE_   /* д��־���ı��뿪��*/
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
		//�ر��������¿�ʼд�����ļ�;
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
		//�ر��������¿�ʼд�����ļ�;
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
		//�ر��������¿�ʼд�����ļ�;
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
//#if _LOG_WRITE_STATE_   /* д��־���ı��뿪��*/
//	Write_Log_Text(log_data);
//#endif _LOG_WRITE_STATE_
//}

//void Logger::Write_Log( int type,MYSTD string& strtext )
//{
//#if _LOG_WRITE_STATE_   /* д��־���ı��뿪��*/
//	assert(LOG_TYPE_ERROR != type);
//	LogData tempdata(type);
//	tempdata.strText = strtext;
//	Write_Log_Text(tempdata);
//#endif _LOG_WRITE_STATE_
//}