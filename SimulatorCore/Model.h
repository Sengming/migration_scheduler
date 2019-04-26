#pragma once
#include"Task.h"
#include"../Containers/Set.h"
#include"../Scheduler.h"
#include"TaskHandler.h"


/***********************************************************************
The Model contains all the information needed to conduct a simulation.
TaskSet: A set of tasks to be executed.

Scheduler: A policy to schedule tasks for excecution.

RunTime: The simulation run time.

ContextSwitch: The time-loss due to interrupting a current task being 
executed and replacing it with another.

ModelName: The name of the model, naming the result report.

modelTaskHandler: A handler for the task set, updating tasks, etc.
*************************************************************************/


class Model
{
	friend class Simulator;
private:
	Set<Task*>* TaskSet;
	Scheduler* scheduler;

	double RunTime;
	double TimeInterval;
	double ContextSwitch;
	std::string ModelName;
	TaskHandler modelTaskHandler;

public:
	Model();
	Model(std::string modelName, Set<Task*>* modelTaskSet, Scheduler* modelScheduler, double modelRunTime);
	Model(std::string modelName, int numberOfPeriodicTasks, int numberOfAperiodicTask, Scheduler* modelScheduler);
	~Model();

	void setModelName(std::string modelName);
	void setScheduler(Scheduler* myScheduler);
	void setTaskSet(Set<Task*>* myTasks);
	void setContextSwitch(double msTime);
	void setTimeInterval(double time);
	void setRunTime(double time);
};

