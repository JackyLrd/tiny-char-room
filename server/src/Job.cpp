#include "../include/Job.hpp"

Job::Job(void* (*process)(void*), void* args): process(process), args(args) {}