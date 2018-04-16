#include "pthread.h"

class WorkerThread
{
	public:
		WorkerThread(int i);
		void* func(void* args);
		void run();
	private:
		pthread_t tid;
};