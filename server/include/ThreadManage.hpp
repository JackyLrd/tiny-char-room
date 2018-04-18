/*
 * @Author: JackyLrd 
 * @Date: 2018-04-18 03:28:21 
 * @Last Modified by: JackyLrd
 * @Last Modified time: 2018-04-18 11:20:04
 */
#ifndef ThreadManage_H_
#define ThreadManage_H_

#include "Job.hpp"
#include "ThreadPool.hpp"

class ThreadManage
{
	public:
		// default thread num is 10
		ThreadManage();
		// set your own thread num
		ThreadManage(int num);
		// put in a job and run
		void run(Job* job);
		// show the jobs num finished by each thread
		void show_thread_state();
		// shut down the pool
		void terminate();
		virtual ~ThreadManage();
	private:
		int num_of_thread;
		ThreadPool* pool;
};

#endif