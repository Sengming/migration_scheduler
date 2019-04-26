#include "RM.h"
#include<iostream>

RM::RM()
{
	preemptive=true;
}


RM::~RM()
{
}

Task* RM::Schedule(double time)
{
	TaskIterator->First();
	Task* bestTask = TaskIterator->CurrentItem();
	double lowestPeriod = -1;
	
	Task* tempTask;
	double tempPeriod;


	for (availableTaskIterator->First(); !availableTaskIterator->IsDone(); availableTaskIterator->Next())
	{
		tempTask = availableTaskIterator->CurrentItem();
		tempPeriod = tempTask->getPeriod();
		if (tempPeriod < lowestPeriod|| lowestPeriod ==-1)
		{
			lowestPeriod = tempPeriod;
			bestTask = tempTask;
		}
	}
	return bestTask;
}