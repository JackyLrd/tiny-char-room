/*
 * @Author: JackyLrd 
 * @Date: 2018-04-18 03:28:35 
 * @Last Modified by: JackyLrd
 * @Last Modified time: 2018-04-18 14:51:56
 */
#ifndef Thread_H_
#define Thread_H_
#include <pthread.h>
#include <list>
#include <assert.h>
#include <cstdio>
#include "Job.hpp"

// thread state
const int THREAD_IDLE = 0;
const int THREAD_WORKING = 1;
const int THREAD_WAITING = 2;
const int THREAD_EXIT = 3;

class Thread
{
	public:
		Thread();
		// getters and setters
		void set_abort();
		void set_id(int id);
		const int get_id();
		const pthread_t get_tid();
		const int get_state();
		const int get_job_count();
		// mutex and condition variable functions
		void lock();
		void unlock();
		void signal();
		void wait();
		// add a job to the job queue
		void add_job(Job* job);
		// the actual function run by the thread
		static void* start_routine(void* args);
		// the thread is created here
		void run();
		// the thread runs this function
		void execute();
	private:
		pthread_mutex_t queue_lock;
		pthread_cond_t queue_ready;
		std::list<Job*> job_queue;
		int state;
		int job_count;
		bool is_abort;
		// the actual thread id
		pthread_t tid;
		// my own thread id
		int thread_id;
};

#endif