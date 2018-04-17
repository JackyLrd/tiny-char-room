#include "../include/ThreadPool.hpp"

ThreadPool::ThreadPool(int thread_num)
{
	state = INIT;
	max_thread_num = thread_num;
	threads = new Thread[thread_num];
	for (unsigned long long i = 0; i < max_thread_num; ++i)
	{
		threads[i].set_tid((pthread_t)i);
		threads[i].run();
	}
	state = RUNNING;
}

void ThreadPool::add_job(Job* job)
{
	int tid = rand() % max_thread_num;
	threads[tid].add_job(job);
}

void ThreadPool::terminate()
{
	state = TERMINATING;

	for (unsigned long long i = 0; i < max_thread_num; ++i)
	{
		threads[i].set_state(ABORT);
	}

	unsigned long long i;
	while (1)
	{
		for (i = 0; i < max_thread_num; ++i)
		{
			assert(threads[i].get_state() != ABORT);
			if (threads[i].get_state() != EXIT)
				break;
		}
		if (i == max_thread_num)
			break;
	}
	
	printf("all threads exit.\n");

	if (threads)
		delete[] threads;
}