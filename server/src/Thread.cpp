#include "../include/Thread.hpp"

Thread::Thread()
{
	pthread_mutex_init(&queue_lock, NULL);
	pthread_cond_init(&queue_ready, NULL);
	state = IDLE;
	job_count = 0;
	is_abort = false;
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

const int Thread::get_job_count()
{
	return job_count;
}

const pthread_t Thread::get_tid()
{
	return tid;
}

void Thread::set_abort()
{
	is_abort = true;
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
		if (state == IDLE)
			state = WAITING;
		printf("thread 0x%lu is waiting for a job.\n", tid);
		while (job_queue.empty())
		{
			if (is_abort)
				break;
			wait();
		}

		if (is_abort)
			break;

		printf("thread 0x%lu starts working.\n", tid);
		assert(!job_queue.empty());

		Job* job = job_queue.front();
		job_queue.pop_front();
		
		unlock();

		if (state == WAITING)
			state = WORKING;

		job->run();
		printf("thread 0x%lu's job finished.\n", tid);
		++job_count;
		delete job;

		if (state == WORKING)
			state = IDLE;
	}

	state = EXIT;
	printf("thread 0x%lu exits.\n", tid);
	pthread_mutex_destroy(&queue_lock);
	pthread_cond_destroy(&queue_ready);
	pthread_exit(NULL);
}