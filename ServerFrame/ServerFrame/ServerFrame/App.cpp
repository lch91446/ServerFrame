#include "stdafx.h"

#include "App.h"
#include "Server.h"
#include <string>

//typedef pugi::xml_node xml_node;
App::App(void)
	//:monsum_query_(io_service_),sum_query_(io_service_)
{
	//检测配置文件;
	
		
	
}

App::~App(void)
{
}

void App::run()
{
	
	slog_.system(MYSTD string("App::run()"));
	slog_.system(MYSTD string("开始引擎加载..."));
	//在这里可以加载其他引擎;
	slog_.system(MYSTD string("完成引擎加载..."));
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
// 	//读取线程数;
// 	xml_node nodeconcurrent = root_node.child("concurrent");
// 	if (!nodeconcurrent.empty())
// 	{
// 		MYSTD string stemp = nodeconcurrent.child_value();
// 		concurrent_ = atoi(stemp.c_str());
// 		slog_.system("读取线程数为 "+stemp);
// 	} 
// 	else
// 	{
// 		MYSTD string stemp ="没有定义线程数，默认为 16 \n";
// 		slog_.system(stemp);
// 		slog_.info(stemp);
// 		concurrent_ = 16;
// 	}
// 	//读取引擎数据库连接ODBC;
// 	xml_node nodecs_intf = root_node.child("cs_intf");
// 	if (!nodecs_intf.empty())
// 	{
// 		cs_intf_ = nodecs_intf.child_value();
// 		slog_.system("引擎数据库连接ODBC "+cs_intf_);
// 	} 
// 	else
// 	{
// 		MYSTD string stemp("引擎数据库连接ODBC 未配置 默认为 zhaoxiaobiao/zhaoxiaobiao@sqlodbc");
// 		slog_.system(stemp);
// 		slog_.info(stemp);
// 		cs_intf_ = "zhaoxiaobiao/zhaoxiaobiao@sqlodbc";
// 	}
// 	//读取监听端口;
// 	xml_node common_node = root_node.child("listenport");
// 	if (!common_node.empty())
// 	{
// 		MYSTD string stemp = common_node.child_value();
// 		port_ = atoi(stemp.c_str());
// 		slog_.system("监听端口为"+stemp);
// 	} 
// 	else
// 	{
// 		MYSTD string stemp("没有读取到数据，默认监听端口为 4566");
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
// 		MYSTD string stemp("没有读取到dbip，默认 127.0.0.1");
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
// 		MYSTD string stemp("没有读取到dbport，默认 4567");
// 		slog_.system(stemp);
// 		slog_.info(stemp);
// 		sum_port_ = "4567";
// 	}
// 	
	return true;
}

