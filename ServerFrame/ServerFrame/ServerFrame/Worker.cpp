#include "stdafx.h"
#include "Worker.h"



Worker::~Worker(void)
{
}

Worker* WorkerManager::get_current_worker() const
{
	Worker* worker = NULL;
	WorkerMap::const_iterator iter(wmap_.find(boost::this_thread::get_id()));
	if(wmap_.end() != iter) worker = iter->second;
	return worker;
}

WorkerManager::WorkerManager()
{

}

WorkerManager::~WorkerManager()
{

}
