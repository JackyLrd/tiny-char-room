#ifndef ThreadManage_H_
#define ThreadManage_H_

#include "Job.hpp"
#include "ThreadPool.hpp"

class ThreadManage
{
	public:
		ThreadManage();
		ThreadManage(int num);
		void run(Job* job);
		void terminate();
		virtual ~ThreadManage();
	private:
		int num_of_thread;
		ThreadPool* pool;
};

#endif