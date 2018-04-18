/*
 * @Author: JackyLrd 
 * @Date: 2018-04-18 02:07:24 
 * @Last Modified by:   JackyLrd 
 * @Last Modified time: 2018-04-18 02:07:24 
 */
#include "../include/ThreadManage.hpp"

ThreadManage::ThreadManage(): num_of_thread(10), pool(new ThreadPool(num_of_thread)) {}

ThreadManage::ThreadManage(int num): num_of_thread(num), pool(new ThreadPool(num_of_thread)) {}

void ThreadManage::run(Job* job)
{
	pool->add_job(job);
}

void ThreadManage::show_thread_state()
{
	pool->show_thread_state();
}

void ThreadManage::terminate()
{
	pool->terminate();
}

ThreadManage::~ThreadManage()
{
	if (pool)
		delete pool;
	pool = NULL;
}