#include "../include/WorkerThread.hpp"

WorkerThread::WorkerThread(int i): tid(i) {}

void* WorkerThread::func(void* args)
{
	pthread_exit(NULL);
}

void WorkerThread::run()
{
	pthread_create(&tid, NULL, func, NULL);
}