#pragma once
#include "../Scheduler.h"

/************************************************
Rate Monotonic scheduler implementation.
Prioritizes the task with the shortest priority.
************************************************/

class RM :
	public Scheduler
{
public:
	RM();
	~RM();
	Task* Schedule(double time);
};

