#pragma once
#include"Task.h"
#include"../Containers/Set.h"
#include"../Monitor/Monitor.h"

/*************************************************************************
TaskHandler manages the task set for the Model, and generates
task pointer sets for the Model if needed.

availableTaskIterator: Iterator that traverses the tasks
available for execution.

TaskIterator: iterates through all the tasks, ready or not.

generateTask(int N, bool PeriodicTasks) generates a task set of
N tasks based on normal distribution. PeriodicTasks indicates 
if the tasks shall be periodic or not.

resetTasks() resets the tasks in the task set after a simulation.

numberOfTasks() return the number of tasks in the task set.

taskAvailable() returns true if there currently exsists a task
ready for execution.

getTaskClosestInTime(double time) returns the task with tarrival
closest in time. 

updateTaskStates(Monitor* myMonitor, double time) updates the task states
corresponding to the current simulation time. 

checkForDeadlineBreaches(Monitor* myMonitor, double time) checks if any of
the tasks have missed their deadlines.
***************************************************************************/



class TaskHandler
{
private:
	AbstractIterator<Task*>* availableTasksIterator;
	AbstractIterator<Task*>* taskIterator;
	int idCounter;
public:
	TaskHandler();
	~TaskHandler();

	Set<Task*> generateTasks(int N, bool PeriodicTasks);

	//initializes the iterators.
	void initialize(AbstractIterator<Task*>* taskIt, AbstractIterator<Task*>* availableTaskIt);
	
	void resetTasks();
	int numberOfTasks();
	bool taskAvailable();
	Task* getTaskClosestInTime(double time);
	void updateTaskStates(Monitor* myMonitor, double time);
	void checkForDeadlineBreaches(Monitor* myMonitor, double time);
};

