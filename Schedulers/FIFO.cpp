#include "FIFO.h"
#include<iostream>

FIFO::FIFO()
{
	lastTaskId = -1;
	preemptive=false;
}


FIFO::~FIFO()
{
}


Task* FIFO::Schedule(double time)
{
	double temp = -1;
	TaskIterator->First();
	Task* bestTask = TaskIterator->CurrentItem();
	Task* tempTask = bestTask;
	for (availableTaskIterator->First(); !availableTaskIterator->IsDone(); availableTaskIterator->Next())
	{
		tempTask = availableTaskIterator->CurrentItem();
		if (tempTask->getTarrival() < temp || temp == -1)
		{
			bestTask = tempTask;
			temp = tempTask->getTarrival();
		}
	}
	return bestTask;
}
