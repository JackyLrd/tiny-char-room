class Job
{
	public:
		void* (*process)(void* args);
		void* args;
};