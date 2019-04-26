#include "EDF.h"


EDF::EDF()
{
	preemptive = true;
}


EDF::~EDF()
{
}


Task* EDF::Schedule(double time)
{
	double tempDeadline = 0;
	
	availableTaskIterator->First();
	Task* bestTask = availableTaskIterator->CurrentItem();
	double closestDeadline = (bestTask->getTarrival() + bestTask->getDeadline())-time;
	for (availableTaskIterator->First(); !availableTaskIterator->IsDone(); availableTaskIterator->Next())
	{
		tempDeadline = ((availableTaskIterator->CurrentItem()->getTarrival() + availableTaskIterator->CurrentItem()->getDeadline()) - time);
		if (tempDeadline < closestDeadline)
		{
			closestDeadline = tempDeadline;
			bestTask = availableTaskIterator->CurrentItem();
		}
	}
	return bestTask;
}

