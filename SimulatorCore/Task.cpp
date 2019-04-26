#include "Task.h"

Task::Task()
{
	FirstTarrival = 0.0;
	Tarrival = 0.0;
	Deadline = 0.0;
	Priority = 0;
	State = IDLE;
	ExecutionTime = 0.0;
	ID = 0;
	Period = 0.0;
	Progression = 0.0;
	Tstarted = 0.0;
	DeadlineMissed = false;
}

Task::Task(int id, double tarrival, double deadline, double executiontime, double period)
{
	FirstTarrival = tarrival;
	Tarrival = tarrival;
	Deadline = deadline;
	Priority = 0;
	State = IDLE;
	ExecutionTime = executiontime;
	ID = id;
	Period = period;
	Progression = 0.0;
	Tstarted = 0.0;
	DeadlineMissed = false;
}

Task::Task(int id)
{
	FirstTarrival = 0.0;
	Tarrival = 0.0;
	Deadline = 0.0;
	Priority = 0;
	State = IDLE;
	ExecutionTime = 0.0;
	ID = id;
	Period = 0.0;
	Progression = 0.0;
	Tstarted = 0.0;
	DeadlineMissed = false;
}

Task::~Task()
{
}


double Task::getTarrival()
{
	return Tarrival;
}

double Task::getDeadline()
{
	return Deadline;
}

double Task::getExecutionTime()
{
	return ExecutionTime;
}

double Task::getRemainingExecutionTime()
{
	return (ExecutionTime - Progression);
}

double Task::getPeriod()
{
	return Period;
}

int Task::getState()
{
	return State;
}

int Task::getID()
{
	return ID;
}


void Task::updateProgressionTime(double time)
{
	Progression += time - Tstarted;
	Tstarted = time;
}