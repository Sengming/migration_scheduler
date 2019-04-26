#pragma once
#include "../Scheduler.h"

/************************************************************
Earliest Deadline First scheduler implementation.
prioritizes the task with the closest deadline.
*************************************************************/
class EDF :
	public Scheduler
{
public:
	EDF();
	~EDF();
	Task* Schedule(double time);
};

