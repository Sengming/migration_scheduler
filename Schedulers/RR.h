#pragma once
#include "../Scheduler.h"

/**********************************************************************
Round Robin scheduler implementation.
Loops throug all tasks, giving them a small time interval to executes.
***********************************************************************/

class RR :
	public Scheduler
{
public:
	RR();
	~RR();
	Task* Schedule(double time);
};

