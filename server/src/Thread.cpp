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

const int Thread::get_state()
{
	return state;
}

void Thread::set_state(int state)
{
	this->state = state;
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
		lock();
		state = WAITING;
		while (job_queue.empty())
		{
			printf("thread 0x%u is waiting for a job.\n", tid);
			wait();
		}

		if (state == ABORT)
			break;

		printf("thread 0x%u starts working.\n", tid);
		assert(!job_queue.empty());

		Job* job = job_queue.front();
		job_queue.pop_front();
		
		unlock();

		state = WORKING;

		job->run();
		printf("thread 0x%u's job finished.\n", tid);
		delete job;

		if (state == ABORT)
			break;

		state = IDLE;
	}

	assert(state == ABORT);
	state = EXIT;
	pthread_mutex_destroy(&queue_lock);
	pthread_cond_destroy(&queue_ready);
	pthread_exit(NULL);
}