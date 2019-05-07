#pragma once
#include"SimulatorCore/Task.h"
#include"Containers/Set.h"

/***********************************************************
MigrationScheduler is the class defining the interface that connects
the implementation of scheduling logic with the simulator.

availableTaskIterator: Iterator that traverses the tasks 
available for execution. Corresponding to a "ready queue."

TaskIterator: iterates through all the tasks, ready or not.

isPreemptive(): Returnes the Preemptive variable value.

setTasks(Set<Task*>* tasks) initialize the iterators.

Schedule(time): A virtual function that must be implemented
by sub-classes, aka. new scheduling policies, containing
the scheduling logic.

************************************************************/


// class MigrationScheduler
// {
// protected:
// 	AbstractIterator<Task*>* availableTaskIterator;
// 	AbstractIterator<Task*>* TaskIterator;
// 	bool preemptive;
// public:
// 	MigrationScheduler();
// 	~MigrationScheduler();
// 	bool isPreemptive();
// 	void setTasks(Set<Task*>* tasks);

// 	virtual Task* Schedule(double time) = 0;
// };

