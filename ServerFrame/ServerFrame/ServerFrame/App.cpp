#include "stdafx.h"

#include "App.h"
#include "Server.h"
#include <string>

//typedef pugi::xml_node xml_node;
App::App(void)
	//:monsum_query_(io_service_),sum_query_(io_service_)
{
	//��������ļ�;
	
		
	
}

App::~App(void)
{
}

void App::run()
{
	
	slog_.system(MYSTD string("App::run()"));
	slog_.system(MYSTD string("��ʼ�������..."));
	//��������Լ�����������;
	slog_.system(MYSTD string("����������..."));
	Server server(io_service_, port_, session_mgr_);
	slog_.system(MYSTD string("server created. "));

	boost::thread_group threads;

	std::ostringstream os;
	os.str().reserve(1024);
	concurrent_ = 16;
 	for(size_t i = 1; i <= concurrent_; ++i)
 	{
		os<<"create thread, #" << i<<"\n";
 		Worker* worker = new Worker(io_service_);
 		worker_mgr_.add_worker(
 			threads.create_thread(boost::bind(&Worker::run, worker)), 
 			worker
 			);
	}
	slog_.system(MYSTD string(os.str()));
 	threads.join_all();
	//io_service_.run();
	return ;
}

bool App::initialize()
{
	if (init_config()&&init_prom())
	{
		return true;
	}
	return false;
}

bool App::init_prom()
{
	{
// 		boost::asio::ip::tcp::resolver resolver(io_service_);
// 		boost::asio::ip::tcp::resolver::query query(sum_server_, sum_port_);
// 		boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
// 		boost::asio::ip::tcp::endpoint endpoint = *iterator;
// 		sum_query_.set_endpoint(endpoint);
// 		slog_.system(MYSTD string("Application::run  SumQuery created."));
// 
// 		monsum_query_.set_endpoint(endpoint);
// 		slog_.system(MYSTD string("Application::run  SumMonQuery created."));
	}
	return true;
}

bool App::init_config()
{
// 	xml_node root_node = m_xml_doc.child("root");
// 	
// 	//��ȡ�߳���;
// 	xml_node nodeconcurrent = root_node.child("concurrent");
// 	if (!nodeconcurrent.empty())
// 	{
// 		MYSTD string stemp = nodeconcurrent.child_value();
// 		concurrent_ = atoi(stemp.c_str());
// 		slog_.system("��ȡ�߳���Ϊ "+stemp);
// 	} 
// 	else
// 	{
// 		MYSTD string stemp ="û�ж����߳�����Ĭ��Ϊ 16 \n";
// 		slog_.system(stemp);
// 		slog_.info(stemp);
// 		concurrent_ = 16;
// 	}
// 	//��ȡ�������ݿ�����ODBC;
// 	xml_node nodecs_intf = root_node.child("cs_intf");
// 	if (!nodecs_intf.empty())
// 	{
// 		cs_intf_ = nodecs_intf.child_value();
// 		slog_.system("�������ݿ�����ODBC "+cs_intf_);
// 	} 
// 	else
// 	{
// 		MYSTD string stemp("�������ݿ�����ODBC δ���� Ĭ��Ϊ zhaoxiaobiao/zhaoxiaobiao@sqlodbc");
// 		slog_.system(stemp);
// 		slog_.info(stemp);
// 		cs_intf_ = "zhaoxiaobiao/zhaoxiaobiao@sqlodbc";
// 	}
// 	//��ȡ�����˿�;
// 	xml_node common_node = root_node.child("listenport");
// 	if (!common_node.empty())
// 	{
// 		MYSTD string stemp = common_node.child_value();
// 		port_ = atoi(stemp.c_str());
// 		slog_.system("�����˿�Ϊ"+stemp);
// 	} 
// 	else
// 	{
// 		MYSTD string stemp("û�ж�ȡ�����ݣ�Ĭ�ϼ����˿�Ϊ 4566");
// 		slog_.system(stemp);
// 		slog_.info(stemp);
// 		port_ = 4566;
// 	}
// 	//dbserver ip;
// 	common_node = root_node.child("dbip");
// 	if (!common_node.empty())
// 	{
// 		sum_server_ = common_node.child_value();
// 		slog_.system("DBIP: "+sum_server_);
// 	} 
// 	else
// 	{
// 		MYSTD string stemp("û�ж�ȡ��dbip��Ĭ�� 127.0.0.1");
// 		slog_.system(stemp);
// 		slog_.info(stemp);
// 		sum_server_ = "127.0.0.1";
// 	}
// 	//dbserver port;
// 	common_node = root_node.child("dbport");
// 	if (!common_node.empty())
// 	{
// 		sum_port_ = common_node.child_value();
// 		//sum_port_ = atoi(stemp.c_str());
// 		slog_.system("DBPORT: "+sum_port_);
// 	} 
// 	else
// 	{
// 		MYSTD string stemp("û�ж�ȡ��dbport��Ĭ�� 4567");
// 		slog_.system(stemp);
// 		slog_.info(stemp);
// 		sum_port_ = "4567";
// 	}
// 	
	return true;
}

