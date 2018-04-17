#include "../include/Thread.hpp"

void Thread::add_job(Job job)
{
	lock();
	add_job(job);
	unlock();
	signal();
}

void Thread::run()
{
	pthread_create(&tid, NULL, start_rountine, this);
}

void Thread::execute()
{
	auto tid = pthread_self();
	while (1)
	{
		pthread_mutex_lock(&queue_lock);
		while (job_queue.empty())
		{
			printf("thread 0x%x is waiting for job.", tid);
			pthread_cond_wait(&queue_ready, &queue_lock);
		}

		printf("thread 0x%x starts working.", tid);
		assert(!job_queue.empty());

		auto job = job_queue.front();
		job_queue.pop_front();
		
		pthread_mutex_unlock(&queue_lock);

		job.process(job.args);
	}

	pthread_exit(NULL);
}

void* Thread::start_rountine(void* args)
{
	Thread* thread = (Thread*)(args);
	thread->execute();
	return NULL;
}