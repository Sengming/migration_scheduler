#pragma once
#include "../Scheduler.h"

/***********************************************************
First In, First Out scheduler implementation.
Prioritizes task from their arrival time.
***********************************************************/

class FIFO :
	public Scheduler
{
private:
	int lastTaskId;
public:
	FIFO();
	~FIFO();
	Task* Schedule(double time);
};

