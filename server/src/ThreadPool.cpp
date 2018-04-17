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
		threads[i].set_abort();
		//printf("after set: %d\n", threads[i].get_state());
		threads[i].signal();
	}

	unsigned long long i;
	while (1)
	{
		for (i = 0; i < max_thread_num; ++i)
		{
			if (threads[i].get_state() != EXIT)
				break;
		}
		// printf("%u\n", i);
		if (i == max_thread_num)
			break;
	}
	
	printf("all threads exit.\n");

	if (threads)
		delete[] threads;

	state = TERMINATED;
}

void ThreadPool::show_thread_state()
{
	int count = 0;
	for (int i = 0; i < max_thread_num; ++i)
	{
		count += threads[i].get_job_count();
		printf("thread 0x%x finished %d jobs, state is %d\n", threads[i].get_tid(), threads[i].get_job_count(), threads[i].get_state());
	}
	printf("%d jobs finished.\n", count);
}