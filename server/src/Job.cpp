#include "../include/Job.hpp"

Job::Job(): process(NULL), args(NULL) {}

Job::Job(void* (*process)(void*), void* args): process(process), args(args) {}