/*
 * @Author: JackyLrd 
 * @Date: 2018-04-18 03:28:25 
 * @Last Modified by: JackyLrd
 * @Last Modified time: 2018-04-18 11:20:30
 */
#ifndef Job_H_
#define Job_H_

// abstract class, inherit it to create your own job, rewrite the run function
class Job
{
	public:
		Job() {}
		virtual void run() = 0;
		virtual ~Job() {}
};

#endif