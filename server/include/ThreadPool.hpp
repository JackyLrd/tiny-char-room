/*
 * @Author: JackyLrd 
 * @Date: 2018-04-18 03:28:17 
 * @Last Modified by: JackyLrd
 * @Last Modified time: 2018-04-18 14:38:30
 */
#ifndef ThreadPool_H_
#define ThreadPool_H_

#include <pthread.h>
#include <cstdio>
#include <assert.h>
#include <cstdlib>
#include "Job.hpp"
#include "Thread.hpp"

// pool state
const int POOL_INIT = 0;
const int POOL_RUNNING = 1;
const int POOL_TERMINATING = 2;
const int POOL_TERMINATED = 3;

class ThreadPool
{
	public:
		// you can only create a pool with thread num
		ThreadPool() = delete;
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool& operator=(const ThreadPool&) = delete;
		ThreadPool(int thread_num);
		// add a job to a thread randomly
		void add_job(Job* job);
		// shut down the pool, all jobs will be done before shut down
		void terminate();
		// show the state of each thread
		void show_thread_state();
	private:
		Thread* threads;
		int max_thread_num;
		int state;
		int ptr;
};

#endif