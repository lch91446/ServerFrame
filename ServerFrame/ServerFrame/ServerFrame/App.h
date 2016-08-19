#pragma once

#include "stdafx.h"
#include "Worker.h"
#include "Session.h"
#include "Connection.h"
#include "logger.h"


class App
{
public:
	App(void);
	~App(void);
	bool initialize();//初始化服务
	void run(); //程序运行函数
	inline Logger& get_logger(){return slog_;}
// 	inline QueryClient& get_QueryClient(){return queryclient;}

	bool init_prom();//初始化程序;
private:
	bool init_config();//初始化配置;
	std::string config_;//配置文件所在的目录;
	std::string module_;
	
	unsigned short port_;//计算服务监听的端口;
	size_t concurrent_;//线程数目;

	Logger slog_;//日志类; 
	boost::asio::io_service io_service_; // io_service唯一实例

	
	//QueryClient queryclient; //链接其他服务器的实例;
	SessionManager session_mgr_;  //会话管理类实例
	WorkerManager worker_mgr_;  //工作线程管理类实例;
	//BillMgr bill_mgr_;   //账单管理类实例;
	//MonSumQuery monsum_query_; //账单数据库更新;

	std::string sum_server_;
	std::string sum_port_;

	std::string cs_intf_;
	
	//pugi::xml_document m_xml_doc;//

};

extern App theapp;