#ifndef Job_H_
#define Job_H_
class Job
{
	public:
		Job() = delete;
		Job(const Job&) = delete;
		Job& operator=(const Job&) = delete;
		Job(void* (*process)(void*), void* args);
		void* (*process)(void*);
		void* args;
};

#endif