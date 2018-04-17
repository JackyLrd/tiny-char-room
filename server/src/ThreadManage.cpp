#include "../include/ThreadManage.hpp"

ThreadManage::ThreadManage(): num_of_thread(10), pool(new ThreadPool(num_of_thread)) {}

ThreadManage::ThreadManage(int num): num_of_thread(num), pool(new ThreadPool(num_of_thread)) {}

void ThreadManage::run(Job* job)
{
	pool->add_job(job);
}

void ThreadManage::terminate()
{
	pool->terminate();
}

ThreadManage::~ThreadManage()
{
	if (pool)
		delete pool;
}