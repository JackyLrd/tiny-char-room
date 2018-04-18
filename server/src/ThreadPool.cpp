#include "../include/ThreadPool.hpp"

// initial all threads
ThreadPool::ThreadPool(int thread_num)
{
	max_thread_num = thread_num;
	state = POOL_INIT;
	ptr = 0;
	threads = new Thread[thread_num];
	for (int i = 0; i < max_thread_num; ++i)
	{
		threads[i].set_id(i);
		threads[i].run();
	}
	state = POOL_RUNNING;
}

void ThreadPool::add_job(Job* job)
{
	// randomly choose a thread
	int id = rand() % max_thread_num;
	threads[id].add_job(job);

	// use round-robin
	// threads[ptr].add_job(job);
	// ptr = (ptr == max_thread_num - 1) ? 0 : ptr + 1;
}

void ThreadPool::terminate()
{
	state = POOL_TERMINATING;

	// wait until all jobs are done
	for (unsigned long long i = 0; i < max_thread_num; ++i)
	{
		threads[i].set_abort();
		threads[i].signal();
		pthread_join(threads[i].get_tid(), NULL);
	}
	
	printf("all threads exit.\n");
	// show thread state
	show_thread_state();

	if (threads)
		delete[] threads;
	threads = NULL;

	state = POOL_TERMINATED;
}

void ThreadPool::show_thread_state()
{
	int count = 0;
	for (int i = 0; i < max_thread_num; ++i)
	{
		// show the number of jobs done by each thread
		count += threads[i].get_job_count();
		printf("thread %d finished %d jobs\n", threads[i].get_id(), threads[i].get_job_count());
	}
	// total jobs done
	printf("%d jobs finished.\n", count);
}