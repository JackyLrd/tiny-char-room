#ifndef ThreadPool_H_
#define ThreadPool_H_

#include <pthread.h>
#include <cstdio>
#include <assert.h>
#include <cstdlib>
#include "Job.hpp"
#include "Thread.hpp"

const int INIT = 0;
const int RUNNING = 1;
const int TERMINATING = 2;
const int TERMINATED = 3;

class ThreadPool
{
	public:
		ThreadPool() = delete;
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool& operator=(const ThreadPool&) = delete;
		ThreadPool(int thread_num);
		void add_job(Job* job);
		void terminate();
		void show_thread_state();
	private:
		Thread* threads;
		int max_thread_num;
		int state;
};

#endif