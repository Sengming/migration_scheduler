#include "Task.h"

Task::Task()
{
	FirstTarrival = 0.0;
	Tarrival = 0.0;
	Deadline = 0.0;
	Priority = 0;
	State = IDLE;
	ExecutionTime = 0.0;
	ExecutionTimeExchangeRate = 1.0;
	ID = 0;
	Period = 0.0;
	Progression = 0.0;
	Tstarted = 0.0;
	DeadlineMissed = false;
}

Task::Task(int id, double tarrival, double deadline, double executiontimehost, double executiontimeremote, double period)
{
	FirstTarrival = tarrival;
	Tarrival = tarrival;
	Deadline = deadline;
	Priority = 0;
	State = IDLE;
	ExecutionTime = executiontimehost;
	ExecutionTimeExchangeRate = executiontimehost/executiontimeremote;
	ID = id;
	Period = period;
	Progression = 0.0;
	Tstarted = 0.0;
	DeadlineMissed = false;
}

Task::Task(int id, double tarrival, double deadline, double executiontimehost, double executiontimeremote, double period, unsigned memoryusage = 2){
	FirstTarrival = tarrival;
	Tarrival = tarrival;
	Deadline = deadline;
	Priority = 0;
	State = IDLE;
	ExecutionTime = executiontimehost;
	ExecutionTimeExchangeRate = executiontimehost/executiontimeremote;
	ID = id;
	Period = period;
	Progression = 0.0;
	Tstarted = 0.0;
	DeadlineMissed = false;
	MemoryUsage = memoryusage;

}


Task::Task(int id)
{
	FirstTarrival = 0.0;
	Tarrival = 0.0;
	Deadline = 0.0;
	Priority = 0;
	State = IDLE;
	ExecutionTime = 0.0;
	ExecutionTimeExchangeRate = 1.0;
	ID = id;
	Period = 0.0;
	Progression = 0.0;
	Tstarted = 0.0;
	DeadlineMissed = false;
}

Task::~Task()
{
}


unsigned Task::get_memory_usage()
{
       return MemoryUsage;
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

void Task::updateExecutionTimeForRemote()
{
	ExecutionTime = getRemainingExecutionTime() * ExecutionTimeExchangeRate;
}
