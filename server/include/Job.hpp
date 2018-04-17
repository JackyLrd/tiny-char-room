class Job
{
	public:
		Job();
		Job(void* (*process)(void*), void* args);
		void* (*process)(void*);
		void* args;
};