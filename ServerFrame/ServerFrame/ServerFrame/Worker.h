#pragma once

#include "stdafx.h"

#include <map>

#include <boost/asio.hpp>
#include <boost/function.hpp>
class Worker
{
public:
	//Worker(void);
	Worker(boost::asio::io_service& io):io_service_(io)
	{};
	~Worker();
	void run() //�߳�ִ�к���
	{
		io_service_.run();
	}
private:
	boost::asio::io_service& io_service_; // APP�е�IO_SERVER
};

class WorkerManager{
public:
	typedef std::map<boost::thread::id, Worker*> WorkerMap; //���߳�ID�� worker�������
public:
	WorkerManager();
	~WorkerManager();
	void add_worker(boost::thread* th, Worker* worker)
	{ wmap_.insert(WorkerMap::value_type(th->get_id(), worker)); };
	Worker* get_current_worker() const;
private:
	WorkerMap wmap_;
};

