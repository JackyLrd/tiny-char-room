#include "../include/Thread.hpp"

Thread::Thread()
{
	pthread_mutex_init(&queue_lock, NULL);
	pthread_cond_init(&queue_ready, NULL);
	state = IDLE;
	tid = 0;
}

void Thread::set_tid(pthread_t tid)
{
	this->tid = tid;
}

void Thread::lock()
{
	pthread_mutex_lock(&queue_lock);
}

void Thread::unlock()
{
	pthread_mutex_unlock(&queue_lock);
}

void Thread::signal()
{
	pthread_cond_signal(&queue_ready);
}

void Thread::wait()
{
	pthread_cond_wait(&queue_ready, &queue_lock);
}

void Thread::add_job(Job* job)
{
	lock();
	job_queue.push_back(job);
	signal();
	unlock();
}

void Thread::run()
{
	pthread_create(&tid, NULL, start_rountine, this);
}

void* Thread::start_rountine(void* args)
{
	Thread* thread = (Thread*)(args);
	thread->execute();
	return NULL;
}

void Thread::execute()
{
	pthread_t tid = pthread_self();
	while (1)
	{
		pthread_mutex_lock(&queue_lock);
		state = WAITING;
		while (job_queue.empty())
		{
			printf("thread 0x%x is waiting for a job.", tid);
			wait();
		}

		printf("thread 0x%x starts working.", tid);
		assert(!job_queue.empty());

		Job* job = job_queue.front();
		job_queue.pop_front();
		
		pthread_mutex_unlock(&queue_lock);

		state = WORKING;

		job->process(job->args);
		printf("thread 0x%x's job finished.");
		delete job;

		state = IDLE;
	}

	pthread_exit(NULL);
}