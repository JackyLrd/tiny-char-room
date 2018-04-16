#include "../include/ThreadPool.hpp"

ThreadPool::ThreadPool(int thread_num)
{
	pthread_mutex_init(&queue_lock, NULL);
	pthread_cond_init(&queue_ready, NULL);
	max_thread_num = thread_num;
	for (int i = 0; i < max_thread_num; ++i)
	{
		threads.push_back(WorkerThread(i));
		threads[i].run();
	}
}