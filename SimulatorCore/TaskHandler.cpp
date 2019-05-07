#include "TaskHandler.h"
#include <random>


TaskHandler::TaskHandler()
{
	idCounter = 1;
}


TaskHandler::~TaskHandler()
{
}

void TaskHandler::initialize(AbstractIterator<Task*>* taskIt, AbstractIterator<Task*>* availableTaskIt)
{
	availableTasksIterator = availableTaskIt;
	taskIterator = taskIt;
}

//Generate taks using normal_distribution 
Set<Task*> TaskHandler::generateTasks(int N, bool PeriodicTasks)
{
	//Create a task set:
	Set<Task*> myTasks;

	for (int i = 0; i < N; ++i)
	{
		myTasks.addItem(new Task);
	}
	
	const int nrolls = 1000000;  // number of experiments
	const int nstars = 200;    // maximum number of stars to distribute

	std::default_random_engine generator;
	
	float a = N / 2;
	float b = N / 4;
	if (a == 0)
	{
		a = 5;
		b = 2;
	}
	if (b == 0)
	{
		b = 2;
	}
	std::normal_distribution<double> distribution(a, b);

	//Use the distribution to provide the task data members with values.
	for (int i = 0; i<nrolls; ++i) 
	{
		double number = distribution(generator);
		if ((number >= 1) && (number <= N + 1))
		{
			myTasks.getItem(int(number) - 1)->ExecutionTime += int(number);
		}
	}


	for (int i = 0; i < N; ++i) 
	{
		myTasks.getItem(i)->ID = idCounter;
		idCounter++;
		myTasks.getItem(i)->ExecutionTime = ceil(((myTasks.getItem(i)->ExecutionTime) * nstars / nrolls));
		myTasks.getItem(i)->Deadline = ceil((myTasks.getItem(i)->ExecutionTime * 3));
		if (PeriodicTasks == true)
		{
			myTasks.getItem(i)->Period = myTasks.getItem(i)->Deadline;
		}
		else
		{
			myTasks.getItem(i)->Period = 0;
		}
		myTasks.getItem(i)->Tarrival = ceil(idCounter*(rand() % 2));
	}
	return myTasks;
}



int TaskHandler::numberOfTasks()
{
	return taskIterator->NumberOfItems();
}


bool TaskHandler::taskAvailable()
{
	availableTasksIterator->First();
	return (!(availableTasksIterator->IsDone()));
}

Task* TaskHandler::getTaskClosestInTime(double time)
{
	double closestTime = -1;
	double tempTime = 0;
	taskIterator->First();
	Task* tempTask;
	Task* bestTask = taskIterator->CurrentItem();
	for (taskIterator->First(); !taskIterator->IsDone(); taskIterator->Next())
	{
		tempTask = taskIterator->CurrentItem();
		tempTime = (tempTask->getTarrival() - time);
		if (tempTime > 0)
		{
			if (tempTime < closestTime || closestTime == -1)
			{
				bestTask = tempTask;
				closestTime = tempTime;
			}
		}
	}
	return bestTask;
}

void TaskHandler::updateTaskStates(Monitor* myMonitor, double time)
{
	Task* tempTask;

	for (taskIterator->First(); !taskIterator->IsDone(); taskIterator->Next())
	{
		tempTask = taskIterator->CurrentItem();

		//If task is finished and periodic, update Arrival Time, and
		//make it wait for the next period to start.
		if (tempTask->State == FINISHED && tempTask->Period != 0)
		{
				tempTask->State = IDLE;
				tempTask->DeadlineMissed = false;
				tempTask->Progression = 0;
				tempTask->Tarrival = tempTask->Tarrival + tempTask->Period;
		}

		if (tempTask->getState() == IDLE)
		{
			if (tempTask->getTarrival() <= time)
			{
				tempTask->State = READY;
				//myMonitor->logArrivalTime(*tempTask, time);
				//myMonitor->logDeadline(*tempTask, (tempTask->getDeadline() + time));
			}
		}
	}
}

void TaskHandler::checkForDeadlineBreaches(Monitor* myMonitor, double time)
{
	Task* tempTask;

	for (availableTasksIterator->First(); !availableTasksIterator->IsDone(); availableTasksIterator->Next())
	{
		tempTask = availableTasksIterator->CurrentItem();
		if (tempTask->getState() != FINISHED && tempTask->DeadlineMissed == false)
		{
			if (time - (tempTask->getTarrival()) > tempTask->getDeadline())
			{
				//Deadline missed!
				//myMonitor->logDeadlineBreach(*tempTask, (tempTask->getTarrival() + tempTask->getDeadline()));
				tempTask->DeadlineMissed = true;
			}
		}
	}
}

void TaskHandler::resetTasks()
{
	Task* tempTask;
	for (taskIterator->First(); !taskIterator->IsDone(); taskIterator->Next())
	{
		tempTask = taskIterator->CurrentItem();
		tempTask->Progression = 0.0;
		tempTask->Tarrival = tempTask->FirstTarrival;
		tempTask->Tstarted = 0.0;
		tempTask->State = IDLE;
	}
}