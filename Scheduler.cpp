#include "Scheduler.h"


Scheduler::Scheduler()
{
}


Scheduler::~Scheduler()
{
}


bool Scheduler::isPreemptive()
{
	return preemptive;
}
void Scheduler::setTasks(Set<Task*>* tasks)
{
	availableTaskIterator = tasks->createAvailableTasksIterator();
	TaskIterator = tasks->createIterator();
}

