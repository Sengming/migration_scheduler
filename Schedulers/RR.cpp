#include "RR.h"
RR::RR()
{
	preemptive = true;
}


RR::~RR()
{
}

Task* RR::Schedule(double time)
{
	Task* tempTask;
	int tempState;

	double N = TaskIterator->NumberOfItems();
	for (availableTaskIterator->First(); !availableTaskIterator->IsDone(); availableTaskIterator->Next())
	{
		tempTask = availableTaskIterator->CurrentItem();
		tempState = tempTask->getState();
		if (tempState == RUNNING)
		{
			availableTaskIterator->Next();
			if (availableTaskIterator->IsDone())
			{
				availableTaskIterator->First();
			}
			return availableTaskIterator->CurrentItem();
		}
	}
	availableTaskIterator->First();
	return availableTaskIterator->CurrentItem();
}
