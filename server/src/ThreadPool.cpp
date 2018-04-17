#include "../include/ThreadPool.hpp"

ThreadPool::ThreadPool(int thread_num)
{
	state = INIT;
	max_thread_num = thread_num;
	threads = new Thread[thread_num];
	for (int i = 0; i < max_thread_num; ++i)
	{
		threads[i].set_tid(i);
		threads[i].run();
	}
	state = RUNNING;
}

void ThreadPool::add_job(Job job)
{
	int tid = rand() % max_thread_num;
	threads[tid].add_job(job);
}

void ThreadPool::terminate()
{

}