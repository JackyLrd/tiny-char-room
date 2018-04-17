#include <pthread.h>
#include <deque>
#include <assert.h>
#include <cstdio>
#include "Job.hpp"

const int IDLE = 0;
const int WORKING = 1;
const int WAITING = 2;
const int ABORT = 3;

class Thread
{
	public:
		Thread();
		static void* start_rountine(void* args);
		void set_tid(pthread_t tid);
		void lock();
		void unlock();
		void signal();
		void wait();
		void add_job(Job job);
		void run();
		void execute();
	private:
		pthread_mutex_t queue_lock;
		pthread_cond_t queue_ready;
		std::deque<Job> job_queue;
		int state;
		pthread_t tid;
};