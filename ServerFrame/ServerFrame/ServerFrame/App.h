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
	bool initialize();//��ʼ������
	void run(); //�������к���
	inline Logger& get_logger(){return slog_;}
// 	inline QueryClient& get_QueryClient(){return queryclient;}

	bool init_prom();//��ʼ������;
private:
	bool init_config();//��ʼ������;
	std::string config_;//�����ļ����ڵ�Ŀ¼;
	std::string module_;
	
	unsigned short port_;//�����������Ķ˿�;
	size_t concurrent_;//�߳���Ŀ;

	Logger slog_;//��־��; 
	boost::asio::io_service io_service_; // io_serviceΨһʵ��

	
	//QueryClient queryclient; //����������������ʵ��;
	SessionManager session_mgr_;  //�Ự������ʵ��
	WorkerManager worker_mgr_;  //�����̹߳�����ʵ��;
	//BillMgr bill_mgr_;   //�˵�������ʵ��;
	//MonSumQuery monsum_query_; //�˵����ݿ����;

	std::string sum_server_;
	std::string sum_port_;

	std::string cs_intf_;
	
	//pugi::xml_document m_xml_doc;//

};

extern App theapp;