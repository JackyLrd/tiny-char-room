#include "../include/Thread.hpp"

Thread::Thread()
{
	pthread_mutex_init(&queue_lock, NULL);
	pthread_cond_init(&queue_ready, NULL);
	state = THREAD_IDLE;
	job_count = 0;
	is_abort = false;
	thread_id = 0;
}

void Thread::set_abort()
{
	is_abort = true;
}

void Thread::set_id(int id)
{
	thread_id = id;
}

const int Thread::get_id()
{
	return thread_id;
}

const pthread_t Thread::get_tid()
{
	return tid;
}

const int Thread::get_state()
{
	return state;
}

const int Thread::get_job_count()
{
	return job_count;
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
	// lock the queue before adding a job
	lock();
	job_queue.push_back(job);
	unlock();
	signal();
}

void* Thread::start_routine(void* args)
{
	Thread* thread = (Thread*)(args);
	thread->execute();
	return NULL;
}

void Thread::run()
{
	// thread is created here
	pthread_create(&tid, NULL, start_routine, this);
}

void Thread::execute()
{
	// pthread_t tid = pthread_self();
	// running loop
	while (1)
	{
		// lock the queue before getting a job
		lock();
		assert(state == THREAD_IDLE);
		state = THREAD_WAITING;
		// printf("thread %d is waiting for a job.\n", thread_id);
		// if the job queue is empty and the pool is running, wait for it
		while (job_queue.empty() && !is_abort)
		{
			wait();
		}

		// if the pool is shutting down and the job queue is empty, exit
		if (is_abort && job_queue.empty())
		{
			// unlock the queue before exit
			unlock();
			break;
		}

		// printf("thread %d starts working.\n", thread_id);
		assert(!job_queue.empty());

		// get a job
		Job* job = job_queue.front();
		job_queue.pop_front();
		
		// unlock the queue after getting a job
		unlock();

		assert(state == THREAD_WAITING);
		state = THREAD_WORKING;

		// run the job
		job->run();
		// printf("thread %d's job finished.\n", thread_id);
		++job_count;
		delete job;
		job = NULL;

		assert(state == THREAD_WORKING);
		state = THREAD_IDLE;
	}

	// release the resources before exiting
	state = THREAD_EXIT;
	printf("thread %d exits.\n", thread_id);
	pthread_mutex_destroy(&queue_lock);
	pthread_cond_destroy(&queue_ready);
	pthread_exit(NULL);
}