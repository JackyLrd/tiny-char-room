#include <pthread.h>
#include <deque>
#include <vector>
#include "Job.hpp"
#include "WorkerThread.hpp"

class ThreadPool
{
	public:
		ThreadPool(int thread_num);
		int add_job(Job job);
		int terminate();
	private:
		pthread_mutex_t queue_lock;
		pthread_cond_t queue_ready;
		std::deque<Job> job_queue;
		std::vector<WorkerThread> threads;
		int max_thread_num;
};