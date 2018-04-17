#ifndef Thread_H_
#define Thread_H_
#include <pthread.h>
#include <list>
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
		void set_tid(pthread_t tid);
		void lock();
		void unlock();
		void signal();
		void wait();
		void add_job(Job* job);
		void run();
		static void* start_rountine(void* args);
		void execute();
	private:
		pthread_mutex_t queue_lock;
		pthread_cond_t queue_ready;
		std::list<Job*> job_queue;
		int state;
		pthread_t tid;
};

#endif